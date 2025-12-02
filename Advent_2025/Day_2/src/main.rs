use std::{io, path::PathBuf};

#[derive(Debug)]
struct Range {
    start: usize,
    end: usize
}

fn read_file(path: PathBuf) -> io::Result<Vec<Range>> {
    let file = std::fs::read_to_string(path)?;

    let ranges: Vec<Range> = file.split(",").map(|x| {
        let (start, end) = x.split_once("-").unwrap();
        Range {start: start.parse::<usize>().unwrap(), end: end.parse::<usize>().unwrap() }
     }).collect();

    Ok(ranges)
}

fn is_invalid_id(id: String) -> bool{
    let (left, right) = id.split_at(id.len() / 2);
    left == right
}

fn is_invalid_id_p2(id: String) -> bool{
    for i in 1..=(id.len() / 2){
        let (left, right) = id.split_at(i);
        let repeated = left.repeat(right.len() / left.len());
        if repeated == right {
            return true;
        }
    }
    false
}

fn solve_part_1(ranges: &Vec<Range>) -> usize{
    let mut sum: usize = 0;
    for range in ranges {
        for i in range.start..=range.end {
            let i_str = i.to_string();
            if is_invalid_id(i_str) {
                sum += i;
            }
        }
    }
    sum
}

fn solve_part_2(ranges: &Vec<Range>) -> usize{
    let mut sum: usize = 0;
    for range in ranges {
        for i in range.start..=range.end {
            let i_str = i.to_string();
            if is_invalid_id_p2(i_str) {
                println!("{i}");
                sum += i;
            }
        }
    }
    sum
}

fn main() -> io::Result<()> {
    let ranges = read_file("input.txt".into())?;
    let p1 = solve_part_1(&ranges);
    let p2 = solve_part_2(&ranges);
    println!("Part 1 {p1}");
    println!("Part 2 {p2}");
    Ok(())
}
