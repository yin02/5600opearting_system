

## Question 1: RAID 0

For **RAID 0**, each operation is targeted at a single disk, as there is no parity disk.

### Write Operations

1. **write, disk 3, LBA 3, length 1**  
2. **write, disk 4, LBA 4, length 1**  
3. **write, disk 0, LBA 5, length 1**  
4. **write, disk 1, LBA 6, length 1**  


### Read Operations

1. **read, disk 2, LBA 4, length 2**
2. **read, disk 3, LBA 6, length 2**
3. **read, disk 4, LBA 8, length 2**
4. **read, disk 0, LBA 0, length 2**
5. **read, disk 1, LBA 2, length 2**
6. **read, disk 2, LBA 4, length 2**

These read operations align with the RAID 0 setup, given that each stripe set distributes data across disks without redundancy.

---

## Question 2: RAID 4

For **RAID 4**, each stripe has parity data on a dedicated parity disk (Disk 4). Each write operation involves writing to both the data disk and the parity disk to maintain consistency.

### Operation 1: `read(LBA=3, length=12)`

The read operations are straightforward, as RAID 4 allows reading directly from the data disks:

- **read, disk 1, LBA=1, len=2**
- **read, disk 2, LBA=1, len=2**
- **read, disk 3, LBA=1, len=2**
- **read, disk 0, LBA=4, len=2**
- **read, disk 1, LBA=4, len=2**
- **read, disk 2, LBA=4, len=2**


### Operation 2: `write(LBA=3, length=12)`

For each data write, we also update the parity disk (Disk 4):

- **write, disk 1, LBA=1, len=2**
- **write, disk 4 (parity), LBA=1, len=2**
- **write, disk 2, LBA=1, len=2**
- **write, disk 4 (parity), LBA=1, len=2**
- **write, disk 3, LBA=1, len=2**
- **write, disk 4 (parity), LBA=1, len=2**
- **write, disk 0, LBA=4, len=2**
- **write, disk 4 (parity), LBA=4, len=2**
- **write, disk 1, LBA=4, len=2**
- **write, disk 4 (parity), LBA=4, len=2**
- **write, disk 2, LBA=4, len=2**
- **write, disk 4 (parity), LBA=4, len=2**

Each data write is followed by an update to the parity disk to reflect the changes.

### Operation 3: `write(LBA=24, length=8)`

Similarly, each write includes parity updates:

- **write, disk 0, LBA=6, len=2**
- **write, disk 4 (parity), LBA=6, len=2**
- **write, disk 1, LBA=6, len=2**
- **write, disk 4 (parity), LBA=6, len=2**
- **write, disk 2, LBA=6, len=2**
- **write, disk 4 (parity), LBA=6, len=2**
- **write, disk 3, LBA=6, len=2**
- **write, disk 4 (parity), LBA=6, len=2**


