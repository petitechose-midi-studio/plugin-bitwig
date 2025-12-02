#!/bin/bash
# Format all C/C++ source files in src/ using clang-format
# Excludes auto-generated protocol files (except Protocol.hpp)

source "$(dirname "${BASH_SOURCE[0]}")/../lib/common.sh"

PROJECT_ROOT="$(find_root)"

# --- Main ---
echo -e "\n${BLUE}═══ Code Formatter ═══${NC}"
echo -e "${DIM}Root: $PROJECT_ROOT${NC}"

cd "$PROJECT_ROOT"

require_cmd clang-format

log "Formatting C/C++ files in src/ (excluding protocol/)..."

count=0

# Format all files except those in src/protocol/
while IFS= read -r -d '' file; do
    clang-format -i "$file"
    ((++count))
done < <(find src -path "src/protocol" -prune -o -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) -print0)

# Format Protocol.hpp (manual file, not auto-generated)
if [[ -f "src/protocol/Protocol.hpp" ]]; then
    clang-format -i "src/protocol/Protocol.hpp"
    ((++count))
    log "Formatted Protocol.hpp (manual file)"
fi

success "Formatted $count files"
