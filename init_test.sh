#!/bin/bash
echo "Running Test cases"
# Step 0: Remove .gatorgit directory if it exists
if [ -d ".gatorgit" ]; then
    rm -r .gatorgit
fi
# Step 1: Compile gatorgit.c
if [ -f "gatorgit.c" ]; then
    gcc -std=c99 main.c gatorgit.c -o gatorgit
else
    echo "Error: gatorgit.c not found."
    exit 1
fi

# Step 2: INIT Test
./gatorgit init > /dev/null 2>&1
if [ -d ".gatorgit" ]; then
    echo "[OK] INIT TEST"
else
    echo "[FAIL] INIT TEST"
fi

# Step 3: ADD Test
./gatorgit add gatorgit.c > /dev/null 2>&1
if grep -q "gatorgit.c" .gatorgit/.index; then
    echo "[OK] ADD TEST"
else
    echo "[FAIL] ADD TEST"
fi

# Step 4: STATUS Test

if [ "$(./gatorgit status | sed -n '3p' | sed 's/^[[:space:]]*//')" = "gatorgit.c" ]; then
    echo "[OK] STATUS TEST"
else
    echo "[FAIL] STATUS TEST"
fi

# Step 5: RM Test
./gatorgit rm gatorgit.c > /dev/null 2>&1
if ! grep -q "gatorgit.c" .gatorgit/.index; then
    echo "[OK] RM TEST"
else
    echo "[FAIL] RM TEST"
fi

# Step 6: COMMIT Test
./gatorgit add gatorgit.c > /dev/null 2>&1
./gatorgit commit -m "GOLDEN GATOR! init" > /dev/null 2>&1
latestId=$(head -n 1 .gatorgit/.prev)
if [ -f ".gatorgit/$latestId/.prev" ] && [ -f ".gatorgit/$latestId/.index" ] && [ -f ".gatorgit/$latestId/.msg" ]; then
    echo "[OK] COMMIT TEST"
else
    echo "[FAIL] COMMIT TEST"
fi

# Remove .gatorgit directory and file gatorgit
rm gatorgit
rm -r .gatorgit
