use std::{io, path::PathBuf};

fn read_file(path: PathBuf) -> io::Result<Vec<String>> {
    let file: Vec<String> = std::fs::read_to_string(path)?.lines().map(|x| x.to_string()).collect();
    Ok(file)
}

fn solve_part_1(lines: Vec<String>) {
    let mut sum = 0u64;
    for line in lines {
        let mut max = 0u64;
        let mut chars: Vec<u32> = line.chars().map(|x| x.to_digit(10).unwrap()).collect();
        let mut start_index = 0;
        for i in 0..12 {
            let mut max_at_i = 0u32;
            for (idx, c) in chars[0..chars.len() - (11 - i)].iter().enumerate() {
                if *c > max_at_i {
                    start_index = idx;
                    max_at_i = *c
                }
            }
            let chars_copy = chars.clone();
            let (left, right) = chars_copy.split_at(start_index + 1);
            chars = right.iter().map(|x| *x).collect();
            
            max = max * 10 + max_at_i as u64;
        }
        println!("{max}");
        sum += max;
    }
    println!("Sum = {sum}");
}

fn main() -> io::Result<()> {
    let lines = read_file("input.txt".into())?;
    solve_part_1(lines);
    Ok(())
}
