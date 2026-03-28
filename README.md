# Randome Range Set(test)

## Description

This program reads a set of integer points and ranges from `points.txt` and `ranges.txt`, then merges them into a minimal set of output ranges based on two constraints:

* Maximum value count per range
* Maximum distance between start and end points

---

## Input

* `points.txt`: one integer per line
* `ranges.txt`: ranges in the format `start:end`

Points are treated as a single value range.

---

## Output

* `output.txt`: merged ranges in the format
  `start_point:end_point:value_count`

---

## Usage

```bash
./Test <max_value_count> <max_point_distance>
```

Example:

```bash
./Test 1000 5000
```

---

## Approach

* All points and ranges are stored as range objects
* Data is sorted by starting point
* A greedy merge is applied:

  * Ranges are merged if they do not exceed the max value count or max distance
* Otherwise, a new range is started

---

## Notes

* Invalid or empty lines in input are ignored
* Value count is calculated as `end - start + 1` for ranges
* Designed to handle large inputs efficiently

---
- Ace
