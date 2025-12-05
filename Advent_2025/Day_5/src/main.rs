use std::{cmp::{max, min}, path::PathBuf};

#[derive(Debug, PartialEq, Clone, Copy, Hash, Eq, PartialOrd, Ord)]
struct IDRange {
    start: usize,
    end: usize,
}

impl IDRange {
    fn contains(&self, id: usize) -> bool {
        id >= self.start && id <= self.end
    }

    fn num_elems(&self) -> usize {
        assert!(self.end >= self.start);
        self.end - self.start + 1
    }
}

fn load_file(path: PathBuf) -> (Vec<IDRange>, Vec<usize>) {
    let file = std::fs::read_to_string(path).unwrap();
    let mut available_ids = false;
    let mut ranges = vec![];
    let mut ids = vec![];
    for line in file.lines() {
        if !available_ids {
            if line.is_empty() {
                available_ids = true;
                continue;
            }
            if let Some((start, end)) = line.split_once("-") {
                ranges.push(IDRange{ start: start.parse::<usize>().expect("Unable to parse usize"), end: end.parse::<usize>().expect("Unable to parse usize") });
            } else {
                println!("Unable to locate - in the current line {line}");
            }
        } else {
            ids.push(line.parse::<usize>().expect("Unable to parse usize"));
        }
    }

    (ranges, ids)
}

fn solve_p1(ranges: &Vec<IDRange>,ids: Vec<usize>) -> usize {
    let mut fresh_ids = 0usize;
    for id in ids {
        for range in ranges.iter() {
            if range.contains(id) {
                fresh_ids += 1;
                break;
            }
        }
    }
    fresh_ids
}

// fn solve_p2(ranges: Vec<IDRange>) -> usize{
//     let mut ranges_cpy: Vec<IDRange> = ranges.iter().clone().map(|x| *x).collect();
//     loop { 
//         let mut new_ranges: Vec<IDRange> = vec![];
//         // let mut indexes_to_be_skipped = vec![];
//         // let mut indexes_to_ignore = vec![];
//         let mut indexes_to_be_skipped = vec![];
//         let mut indexes_to_ignore = vec![];
//         for (idx, range) in ranges_cpy.iter().enumerate() {
//             let mut found = false;
//             if indexes_to_be_skipped.contains(&idx) || indexes_to_ignore.contains(&idx) {
//                 continue;
//             }
//             for (idx_2, range_2) in ranges_cpy[idx + 1..].iter().enumerate() {
//                 if indexes_to_be_skipped.contains(&idx_2) || indexes_to_ignore.contains(&idx_2) {
//                    continue;
//                 }
//                 if range.contains(range_2.start) && range.contains(range_2.end) {
//                     indexes_to_ignore.push(idx_2);
//                 } else if range.contains(range_2.start) {
//                     new_ranges.push(IDRange { start: range.start, end: max(range.end,range_2.end)});
//                     indexes_to_be_skipped.push(idx_2);
//                     found = true;
//                     break;
//                 } else if range.contains(range_2.end) {
//                     new_ranges.push(IDRange { start: min(range.start, range_2.start), end: range.end});
//                     indexes_to_be_skipped.push(idx_2);
//                     found = true;
//                     break;
//                 }


//                 // if range_2.is_in_range(range.end) {
//                 //     new_ranges.push(IDRange { start: min(range.start, range_2.start), end: range_2.end });
//                 //     found = true;
//                 //     indexes_to_be_skipped.push(idx_2);
//                 //     break;
//                 // }
//             }
//             if !found && !indexes_to_be_skipped.contains(&idx){
//                 new_ranges.push(*range);
//             }
//         }

//         if new_ranges == ranges_cpy {
//             break;
//         }
//         ranges_cpy = new_ranges;    
//     }
//     println!("{ranges_cpy:?}");
//     let mut sum = 0;
//     for range in ranges_cpy.iter() {
//         for range_2 in ranges_cpy.iter() {
//             if range == range_2 {
//                 continue;
//             }
//             if range_2.contains(range.start) || range_2.contains(range.end) {
//                 println!("Wtf {range:?} {range_2:?}");
//             }
//         }
//         sum += range.num_elems();
//     }

//     sum
// }


fn solve_p2(ranges: Vec<IDRange>) -> usize{
    let mut ranges_cpy: Vec<IDRange> = ranges.iter().clone().map(|x| *x).collect();
    ranges_cpy.sort_by(|a, b| (a.start).cmp(&b.start));
    let ranges_cpy_2: Vec<IDRange> = ranges_cpy.iter().clone().map(|x| *x).collect();
    for (idx, range) in ranges_cpy.iter_mut().enumerate() {
        for (idx_2, range_2) in ranges_cpy_2[idx + 1..].iter().enumerate() {
            if range.contains(range_2.start) && range.contains(range_2.end) { // |R1 start |R2 start |R2 end |R1 end // Caused bugs
               
            } else if range_2.contains(range.end) { // |R1|R2|R1|R2 -> R1start | R2end
                range.end = max(range.end, range_2.end);
            } else if !range_2.contains(range.end) {
                break;
            } else if range_2.contains(range.start) {
                range.end = max(range.end, range_2.end);
            }
        }
    }
    ranges_cpy.sort_by(|a, b| a.start.cmp(&b.start));
    println!("{ranges_cpy:?}");

    let mut to_be_deleted = vec![];
    for (idx, range) in ranges_cpy.iter().enumerate() {
        for (idx_2, range_2) in ranges_cpy.iter().enumerate() {
            if idx_2 <= idx{
                continue;
            }
            if range.contains(range_2.start) && range.contains(range_2.end) {
                if !to_be_deleted.contains(&idx_2) {
                    to_be_deleted.push(idx_2);
                }
            }
        }
    }
    
    println!("{to_be_deleted:?}");
    println!("{}/{}", to_be_deleted.len(), ranges_cpy.len());

    for idx in to_be_deleted.iter().rev() {
        ranges_cpy.remove(*idx);
    }

    let mut sum = 0;
    for range in ranges_cpy.iter() {
        for range_2 in ranges_cpy.iter() {
            if range == range_2 {
                continue;
            }
        }
        sum += range.num_elems();
    }

    sum

}

fn main() {
    let (ranges, ids) = load_file("input.txt".into());
    // println!("{ranges:?}");
    // println!("{ids:?}");
    let fresh_ids = solve_p1(&ranges, ids);
    println!("P1 = {fresh_ids}");
    let sum = solve_p2(ranges);
    println!("P2 = {sum}");
    let a: usize = 838938140710707;
    let b: usize = 390838598984693; // Too high
    let c: usize = 450407223820408;
    let d: usize = 424695343945672;
    let k: usize = 49419487207402;
    let d: usize = 41530844825098; // Too low
                   //355555479253787 -- Correct Answer
                   //356102667326406
}
