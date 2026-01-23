#!/bin/bash

# Test runner for Issembly VM
# Runs all positive and negative test cases

BINARY="${1:-./../build/custom-mini-vm}"
TEST_DIR="$(cd "$(dirname "$0")" && pwd)"
PASSED=0
FAILED=0

echo "==================================="
echo "Running Issembly VM Test Suite"
echo "==================================="
echo ""

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if binary exists
if [ ! -f "$BINARY" ]; then
    echo -e "${RED}Error: Binary not found at $BINARY${NC}"
    exit 1
fi

echo -e "${YELLOW}Binary: $BINARY${NC}"
echo ""

# ============================================
# POSITIVE TESTS (should run successfully)
# ============================================
echo -e "${YELLOW}--- POSITIVE TESTS ---${NC}"

for test_file in "$TEST_DIR/positive"/*.txt; do
    test_name=$(basename "$test_file")
    echo -n "Testing $test_name ... "
    
    if "$BINARY" "$test_file" > /dev/null 2>&1; then
        echo -e "${GREEN}PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}FAIL${NC}"
        ((FAILED++))
    fi
done

echo ""

# ============================================
# NEGATIVE TESTS (should fail gracefully)
# ============================================
echo -e "${YELLOW}--- NEGATIVE TESTS ---${NC}"

for test_file in "$TEST_DIR/negative"/*.txt; do
    test_name=$(basename "$test_file")
    echo -n "Testing $test_name (should fail) ... "
    
    if "$BINARY" "$test_file" > /dev/null 2>&1; then
        echo -e "${RED}FAIL (should have errored)${NC}"
        ((FAILED++))
    else
        echo -e "${GREEN}PASS (correctly rejected)${NC}"
        ((PASSED++))
    fi
done

echo ""
echo "==================================="
echo -e "Results: ${GREEN}$PASSED passed${NC}, ${RED}$FAILED failed${NC}"
echo "==================================="

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
