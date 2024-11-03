
### Question 1 (20 Points)
**Problem:**  
A disk rotates at 7200 RPM and can transfer data at 150 MB/s from its outer track. Determine the maximum number of bytes that a single outer track can hold.

**Solution:**

1. **Calculate the time for one rotation**:
   - The disk rotates at 7200 RPM (Revolutions Per Minute).
   - Convert RPM to RPS (Revolutions Per Second):
     $$
     7200 \, \text{RPM} = \frac{7200}{60} = 120 \, \text{RPS}
     $$
   - The time for one rotation is the reciprocal of the RPS:
     $$
     \text{Time per rotation} = \frac{1}{120} \approx 0.00833 \, \text{seconds} \, \text{(or 8.33 ms)}
     $$

2. **Calculate the maximum amount of data per rotation**:
   - The disk has a transfer rate of 150 MB/s.
   - Data transferred per rotation:
     $$
     \text{Data per rotation} = 150 \, \text{MB/s} \times 0.00833 \, \text{s} = 1.25 \, \text{MB}
     $$
   - Converting MB to bytes (1 MB = \( 10^6 \) bytes):
     $$
     \text{Data per rotation} = 1.25 \times 10^6 \, \text{bytes} = 1,250,000 \, \text{bytes}
     $$

**Answer:**  
The maximum number of bytes that a single outer track can hold is **1,250,000 bytes**.

---

### Question 2 (40 Points)
**Problem:**  
For two different cases, calculate the average time for a 65536-byte random read request.

#### Case 1:
- Average seek time = 5 ms
- Rotational speed = 10,000 RPM
- Transfer rate = 200 MB/s

#### Solution for Case 1:

1. **Average rotational latency**:
   - Convert RPM to RPS:
     $$
     10,000 \, \text{RPM} = \frac{10,000}{60} \approx 166.67 \, \text{RPS}
     $$
   - Time for one rotation:
     $$
     \text{Time per rotation} = \frac{1}{166.67} \approx 0.006 \, \text{seconds} = 6 \, \text{ms}
     $$
   - Average rotational latency (half of one rotation):
     $$
     \text{Average rotational latency} = \frac{6}{2} = 3 \, \text{ms}
     $$

2. **Data transfer time**:
   - Request size = 65536 bytes = 65.536 KB = 0.065536 MB
   - Transfer rate = 200 MB/s
   - Transfer time:
     $$
     \text{Transfer time} = \frac{0.065536 \, \text{MB}}{200 \, \text{MB/s}} = 0.00032768 \, \text{seconds} = 0.33 \, \text{ms}
     $$

3. **Total time**:
   $$
   \text{Total time} = \text{Seek time} + \text{Rotational latency} + \text{Transfer time}
   $$
   $$
   = 5 \, \text{ms} + 3 \, \text{ms} + 0.33 \, \text{ms} = 8.33 \, \text{ms}
   $$

#### Case 2:
- Average seek time = 12 ms
- Rotational speed = 5400 RPM
- Transfer rate = 200 MB/s

#### Solution for Case 2:

1. **Average rotational latency**:
   - Convert RPM to RPS:
     $$
     5400 \, \text{RPM} = \frac{5400}{60} = 90 \, \text{RPS}
     $$
   - Time for one rotation:
     $$
     \text{Time per rotation} = \frac{1}{90} \approx 0.0111 \, \text{seconds} = 11.1 \, \text{ms}
     $$
   - Average rotational latency:
     $$
     \text{Average rotational latency} = \frac{11.1}{2} \approx 5.55 \, \text{ms}
     $$

2. **Data transfer time** (same as above):
   $$
   \text{Transfer time} = 0.33 \, \text{ms}
   $$

3. **Total time**:
   $$
   \text{Total time} = 12 \, \text{ms} + 5.55 \, \text{ms} + 0.33 \, \text{ms} = 17.88 \, \text{ms}
   $$

**Answers:**
- **Case 1:** 8.33 ms
- **Case 2:** 17.88 ms

---

### Question 3 (20 Points)
**Problem:**  
If a system has an estimated mean time between failures (MTBF) of 20,000 days, what is its expected annual reliability?

**Solution:**

1. **Convert MTBF to annual failure probability**:
   - MTBF = 20,000 days
   - Convert MTBF to years:
     $$
     \text{MTBF (years)} = \frac{20,000 \, \text{days}}{365 \, \text{days/year}} \approx 54.79 \, \text{years}
     $$

2. **Calculate annual failure probability**:
   - Annual failure rate \( R \) is the reciprocal of MTBF:
     $$
     R = \frac{1}{54.79} \approx 0.01825
     $$

3. **Calculate reliability**:
   - Annual reliability \( = 1 - R \):
     $$
     \text{Reliability} = 1 - 0.01825 \approx 0.98175
     $$

**Answer:**  
The system's expected annual reliability is approximately **98.18%**.

---

### Question 4 (20 Points)
**Problem:**  
Determine if a system meets the required availability of at least 99.995% given:
- Mean Time to Restore (MTTR) = 1.5 hours
- Mean Time Between Failure (MTBF) = 3.1667 years

**Solution:**
Given:
$$
\text{MTBF} = 3.1667 \, \text{years}
$$

Convert MTBF to hours:
$$
\text{MTBF (hours)} = 3.1667 \times 365 \times 24 = 27740.292 \, \text{hours}
$$

Now, let’s recalculate the availability with this corrected MTBF:

1. **Calculate Availability**:
   $$
   \text{Availability} = \frac{\text{MTBF}}{\text{MTBF} + \text{MTTR}} = \frac{27740.292}{27740.292 + 1.5} \approx 0.999946
   $$

2. **Convert to percentage**:
   $$
   \text{Availability percentage} = 0.999946 \times 100 \approx 99.9946\%
   $$

**Conclusion**:  
The corrected availability calculation confirms that the system’s availability is **99.9946%**, which still falls slightly below the required 99.995%, meaning the system **does not meet** the availability threshold.