## Question 1: Page tables
```
Page Directory (Block 00000)
-----------------------------------------
| Index (i) | Writable (w) | Physical Block |
-----------------------------------------
| 080       | Ignored      | 00001          --> Points to Page Table at 00001
| 100       | Ignored      | 00002          --> Points to Page Table at 00002

-----------------------------------------

Page Table at Block 00001 (for 0080C000 to 0080EFFF - Read-Only)
-----------------------------------------
| Index (i) | Writable (w) | Physical Block |
-----------------------------------------
| 00C       | 0            | 00003          --> Points to Physical Block 00003 (Page 0080C)
| 00D       | 0            | 00004          --> Points to Physical Block 00004 (Page 0080D)
| 00E       | 0            | 00005          --> Points to Physical Block 00005 (Page 0080E)

-----------------------------------------

Page Table at Block 00002 (for 0100A000 to 0100CFFF - Read/Write)
-----------------------------------------
| Index (i) | Writable (w) | Physical Block |
-----------------------------------------
| 00A       | 1            | 00006          --> Points to Physical Block 00006 (Page 0100A)
| 00B       | 1            | 00007          --> Points to Physical Block 00007 (Page 0100B)
| 00C       | 1            | 00008          --> Points to Physical Block 00008 (Page 0100C)

-----------------------------------------
```

### Explanation of Arrows:
- In the **Page Directory** at block `00000`:
  - Entry `080` points to **Page Table** at `00001`.
  - Entry `100` points to **Page Table** at `00002`.

- In **Page Table** at block `00001` (Read-Only entries):
  - Entry `00C` maps to **Physical Block** `00003`.
  - Entry `00D` maps to **Physical Block** `00004`.
  - Entry `00E` maps to **Physical Block** `00005`.

- In **Page Table** at block `00002` (Read/Write entries):
  - Entry `00A` maps to **Physical Block** `00006`.
  - Entry `00B` maps to **Physical Block** `00007`.
  - Entry `00C` maps to **Physical Block** `00008`.



---

## Question 2: Page faulting (50 Points)
### Execution Steps

1. **Instruction Fetch (00000, FFC):**
   - **Action:** Fetch instruction at address `00000,FFC`.
   - **Page Fault:** Page at `00000,FFC` is not present in memory.
   - **Page Allocation:** Allocate physical page `00001`.
   - **Page Directory Update:** Set entry `00000[0]` to point to physical page `00001`.
   - **Read Block:** Load page 0 of `/bin/program` into physical page `00001`.
   - **Return from Fault.**

2. **Instruction Fetch (00000, FFC):**
   - **Action:** Fetch instruction at address `00000,FFC`.
   - **Attempt Instruction:** `PUSH #10`.

3. **Execute PUSH #10:**
   - **SP Decrement:** Update `SP = 0C000,FFC`.
   - **Store (0C000, FFC):** Attempt to store value `10` at address `0C000,FFC`.
   - **Page Fault:** Page at `0C000,FFC` is not present (stack page not allocated).
   - **Page Allocation:** Allocate physical page `00002` for the stack.
   - **Page Directory Update:** Set entry `00000[3072]` to point to physical page `00002` (0xC00 maps to page 3072 in the directory).
   - **Return from Fault.**

4. **Continue PUSH #10 Execution:**
   - **Store (0C000, FFC):** Store value `10` at address `0C000,FFC`.
   - **Success:** `PUSH #10` completed successfully.

5. **Instruction Fetch (00001,000):**
   - **Action:** Fetch instruction at address `00001,000`.
   - **Page Fault:** Page at `00001,000` is not present (new code page not in memory).
   - **Page Allocation:** Allocate physical page `00003`.
   - **Page Directory Update:** Set entry `00000[1]` to point to physical page `00003`.
   - **Read Block:** Load page 1 of `/bin/program` into physical page `00003`.
   - **Return from Fault.**

6. **Instruction Fetch (00001,000):**
   - **Action:** Fetch instruction at address `00001,000`.
   - **Attempt Instruction:** `CALL 2,000`.

7. **Execute CALL 2,000:**
   - **SP Decrement:** Update `SP = 0C000,FF8`.
   - **Store (0C000,FF8):** Attempt to store return address (`00001,004`) at `0C000,FF8`.
   - **Success:** Write return address to `0C000,FF8`.
   - **Set PC:** Update `PC = 00002,000`.
   - **Success:** `CALL 2,000` completed successfully.

8. **Instruction Fetch (00002,000):**
   - **Action:** Fetch instruction at address `00002,000`.
   - **Page Fault:** Page at `00002,000` is not present (new code page not loaded).
   - **Page Allocation:** Allocate physical page `00004`.
   - **Page Directory Update:** Set entry `00000[2]` to point to physical page `00004`.
   - **Read Block:** Load page 2 of `/bin/program` into physical page `00004`.
   - **Return from Fault.**

9. **Instruction Fetch (00002,000):**
   - **Action:** Fetch instruction at address `00002,000`.
   - **Attempt Instruction:** `MOV EAX → *(10,000)`.

10. **Execute MOV EAX → *(10,000):**
    - **Store (10,000):** Attempt to store value of `EAX` at address `10,000`.
    - **Page Fault:** Page at `10,000` is not present (data page not allocated).
    - **Page Allocation:** Allocate physical page `00005`.
    - **Page Directory Update:** Set entry `00000[16]` to point to physical page `00005`.
    - **Return from Fault.**

11. **Continue MOV EAX → *(10,000):**
    - **Store (10,000):** Write value of `EAX` to address `10,000`.
    - **Success:** `MOV EAX → *(10,000)` completed successfully.

12. **Instruction Fetch (00002,004):**
    - **Action:** Fetch instruction at address `00002,004`.
    - **Attempt Instruction:** `HALT`.
    - **Success:** `HALT` executed successfully.


