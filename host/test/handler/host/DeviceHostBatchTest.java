package handler.host;

import com.bitwig.extension.callback.BooleanValueChangedCallback;
import com.bitwig.extension.callback.DoubleValueChangedCallback;
import com.bitwig.extension.callback.IntegerValueChangedCallback;
import com.bitwig.extension.callback.StringValueChangedCallback;
import com.bitwig.extension.controller.api.*;
import config.BitwigConfig;
import handler.controller.DeviceController;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.mockito.ArgumentCaptor;
import protocol.Protocol;
import protocol.struct.DevicePageChangeMessage;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

class DeviceHostBatchTest {
    private final ControllerHost host = mock(ControllerHost.class);
    private final Protocol protocol = mock(Protocol.class);
    private final CursorRemoteControlsPage page = mock(CursorRemoteControlsPage.class, RETURNS_DEEP_STUBS);
    private final List<Scheduled> tasks = new ArrayList<>();
    private final List<Batch> batches = new ArrayList<>();
    private final StringValueChangedCallback[] displays = new StringValueChangedCallback[BitwigConfig.MAX_PARAMETERS];
    private final DoubleValueChangedCallback[] modulation = new DoubleValueChangedCallback[BitwigConfig.MAX_PARAMETERS];
    private final BooleanValueChangedCallback[] automation = new BooleanValueChangedCallback[BitwigConfig.MAX_PARAMETERS];
    private DeviceHost subject;

    private record Scheduled(Runnable action, long delay) {}
    // Protocol consumes reused arrays synchronously: retain their contents at send time.
    private record Batch(int dirty, int echo, int automated, float[] values, float[] modulated, String[] display) {}

    @BeforeEach
    void setup() {
        doAnswer(call -> {
            tasks.add(new Scheduled(call.getArgument(0), call.getArgument(1)));
            return null;
        }).when(host).scheduleTask(any(Runnable.class), anyLong());
        doAnswer(call -> {
            assertEquals(0, (int) call.getArgument(0));
            batches.add(new Batch(call.getArgument(1), call.getArgument(2), call.getArgument(3),
                    ((float[]) call.getArgument(4)).clone(), ((float[]) call.getArgument(5)).clone(),
                    ((String[]) call.getArgument(6)).clone()));
            return null;
        }).when(protocol).deviceRemoteControlsBatch(anyInt(), anyInt(), anyInt(), anyInt(),
                any(float[].class), any(float[].class), any(String[].class));
        subject = new DeviceHost(host, protocol, mock(CursorTrack.class, RETURNS_DEEP_STUBS),
                mock(CursorDevice.class, RETURNS_DEEP_STUBS), page, mock(DeviceBank.class, RETURNS_DEEP_STUBS));
        subject.setupObservers();
        for (int i = 0; i < BitwigConfig.MAX_PARAMETERS; i++) {
            RemoteControl parameter = page.getParameter(i);
            var display = ArgumentCaptor.forClass(StringValueChangedCallback.class);
            verify(parameter.value().displayedValue()).addValueObserver(display.capture());
            displays[i] = display.getValue();
            var modulated = ArgumentCaptor.forClass(DoubleValueChangedCallback.class);
            verify(parameter.modulatedValue()).addValueObserver(modulated.capture());
            modulation[i] = modulated.getValue();
            var automated = ArgumentCaptor.forClass(BooleanValueChangedCallback.class);
            verify(parameter.hasAutomation()).addValueObserver(automated.capture());
            automation[i] = automated.getValue();
        }
    }

    private void runTask(long delay) {
        int index = -1;
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks.get(i).delay() == delay) { index = i; break; }
        }
        assertTrue(index >= 0, "Missing scheduled task at " + delay + "ms");
        tasks.remove(index).action().run();
    }

    private void value(int index, double value, String display) {
        when(page.getParameter(index).value().get()).thenReturn(value);
        displays[index].valueChanged(display);
    }

    @Test
    void coalescesLatestValuesAndClearsOnlyDirtyAndEchoMasksAfterSend() {
        DeviceController controller = mock(DeviceController.class);
        when(controller.consumeEcho(3)).thenReturn(true);
        subject.setDeviceController(controller);
        value(1, 0.25, "25%");
        value(1, 0.5, "50%");
        value(3, 0.75, "75%");
        automation[3].valueChanged(true);
        runTask(1);
        assertEquals(1, batches.size());
        Batch first = batches.get(0);
        assertEquals(0b1010, first.dirty());
        assertEquals(0b1000, first.echo());
        assertEquals(0b1000, first.automated());
        assertEquals(0.5f, first.values()[1]);
        assertEquals(0.75f, first.values()[3]);
        assertEquals("50%", first.display()[1]);
        assertEquals("", first.display()[0]);
        assertArrayEquals(first.values(), first.modulated());
        runTask(1);
        assertEquals(1, batches.size(), "Idle timer must not send");
        value(2, 0.125, "12.5%");
        runTask(1);
        Batch second = batches.get(1);
        assertEquals(0b0100, second.dirty());
        assertEquals(0, second.echo());
        assertEquals(0b1000, second.automated());
        assertEquals(0.5f, second.values()[1]);
    }

    @Test
    void hiddenModulationDoesNotRepaintAndRevealUsesCurrentModulation() {
        value(2, 0.25, "25%");
        runTask(1);
        modulation[2].valueChanged(0.75);
        runTask(1);
        assertEquals(1, batches.size());
        when(page.getParameter(2).modulatedValue().get()).thenReturn(0.8);
        subject.setParameterModulationVisible(2, true);
        runTask(1);
        assertEquals(0.25f, batches.get(1).values()[2]);
        assertEquals(0.8f, batches.get(1).modulated()[2]);
        subject.setParameterModulationVisible(2, false);
        runTask(1);
        assertEquals(0.25f, batches.get(2).modulated()[2]);
        subject.setParameterModulationVisible(-1, true);
        subject.setParameterModulationVisible(BitwigConfig.MAX_PARAMETERS, true);
        runTask(1);
        assertEquals(3, batches.size());
    }

    @Test
    void suppressedViewsAndSelectorsRetainPendingValuesUntilResumed() {
        subject.setControllerViewState(1, false);
        value(0, 0.25, "25%");
        runTask(1);
        assertTrue(batches.isEmpty());
        subject.setControllerViewState(0, true);
        value(0, 0.75, "75%");
        runTask(1);
        assertTrue(batches.isEmpty());
        subject.setControllerViewState(0, false);
        runTask(1);
        assertEquals(1, batches.size());
        assertEquals(1, batches.get(0).dirty());
        assertEquals(0.75f, batches.get(0).values()[0]);
        assertEquals("75%", batches.get(0).display()[0]);
    }

    @Test
    void replacementPageDoesNotReplayOldDisplayOrEcho() {
        DeviceController controller = mock(DeviceController.class);
        when(controller.consumeEcho(0)).thenReturn(true);
        subject.setDeviceController(controller);
        value(0, 0.25, "old");
        when(page.getParameter(0).value().get()).thenReturn(0.75);
        when(page.getParameter(0).value().displayedValue().get()).thenReturn("new");
        var changed = ArgumentCaptor.forClass(IntegerValueChangedCallback.class);
        verify(page.selectedPageIndex()).addValueObserver(changed.capture());
        changed.getValue().valueChanged(1);
        // Rapid page navigation must publish just the final snapshot.
        when(page.selectedPageIndex().get()).thenReturn(2);
        when(page.pageCount().get()).thenReturn(3);
        when(page.getName().get()).thenReturn("Replacement");
        changed.getValue().valueChanged(2);
        assertEquals(1, tasks.stream().filter(task -> task.delay() == BitwigConfig.PAGE_CHANGE_MS).count());
        runTask(BitwigConfig.PAGE_CHANGE_MS);
        var info = ArgumentCaptor.forClass(DevicePageChangeMessage.PageInfo.class);
        var controls = ArgumentCaptor.forClass(DevicePageChangeMessage.RemoteControls[].class);
        verify(protocol).devicePageChange(info.capture(), controls.capture());
        assertEquals(2, info.getValue().getDevicePageIndex());
        assertEquals("Replacement", info.getValue().getDevicePageName());
        assertEquals(BitwigConfig.MAX_PARAMETERS, controls.getValue().length);
        assertEquals("new", controls.getValue()[0].getDisplayValue());
        assertEquals(0.75f, controls.getValue()[0].getParameterValue());
        runTask(1);
        assertEquals(1, batches.size());
        assertEquals(0.75f, batches.get(0).values()[0]);
        assertEquals("new", batches.get(0).display()[0]);
        assertEquals(0, batches.get(0).dirty(), "Full page already published the values");
        assertEquals(0, batches.get(0).echo(), "Old page controller echo must not leak");
    }
}
