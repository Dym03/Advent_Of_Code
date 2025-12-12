use core::num;
use std::{path::PathBuf, sync::{Arc, atomic::{AtomicU64, AtomicUsize}}};

use utils::{Grid, GridTileError};

#[derive(Debug, Clone)]
struct Present {
    row: usize,
    col: usize,
    shape: Vec<Vec<char>>      
}

impl Present {
    fn print(&self) {
        for y in 0..self.row {
            for x in 0..self.col {
                print!("{}", self.shape[y][x]);
            }
            println!();
        }
    }
}

fn parse_preset(present_str: &str) -> Present {
    let mut present = Present{ row: 0, col: 0, shape: vec![] };
    for line in present_str.lines().skip(1) {
        present.shape.push(line.chars().collect());
    }
    present.row = present.shape.len();
    present.col = present.shape.first().unwrap().len();
    present
}

#[derive(Debug)]
struct Region {
    width: usize,
    height: usize,
    quantities: Vec<usize>
}

fn parse_region(region_str: &str) -> Region {
    let (wh, quantities) = region_str.split_once(":").unwrap();
    let (w, h) = wh.split_once("x").unwrap();
    let quantities: Vec<usize> = quantities.split_ascii_whitespace()
                                            .map(|x| x.parse::<usize>().expect("Unable to parse usize"))
                                            .collect();
    Region { width: w.parse::<usize>().unwrap(), height: h.parse::<usize>().unwrap(), quantities }
}

fn read_input(path: PathBuf) -> (Vec<Present>, Vec<Region>) {
    let file = std::fs::read_to_string(path).expect("Unable to parse from this path");
    let parts: Vec<&str> = file.split("\n\n").collect();
    let mut presents = vec![];
    let num_presents = parts.len() - 1;
    let mut regions = vec![];
    for (idx, part) in parts.into_iter().enumerate(){
        if idx < num_presents {
            let present = parse_preset(part);
            println!("{present:?}");
            presents.push(present);
        } else {
            for r in part.lines() {
                let region = parse_region(r);
                println!("{region:?}");
                regions.push(region);
            }
        }
    }
    (presents, regions)
}

enum GridTile {
    Occupied,
    Free
}

impl ToString for GridTile {
    fn to_string(&self) -> String {
        match self {
            GridTile::Occupied => "#".to_string(),
            GridTile::Free => ".".to_string(),
        }
    }
}

fn prepare_grid(reg: &Region) -> Grid<GridTile> {
    let mut grid = Grid::<GridTile>::new();
    for y in 0..reg.height {
        let mut row = vec![];
        for x in 0..reg.width {
            row.push(GridTile::Free);
        }
        grid.add_row(row);
    }
    grid
}

fn add_present(grid: &mut Grid<GridTile>, present: &Present, row: usize, col: usize) -> bool {
    for y in row..row+present.row {
        for x in col..col+present.col {
            if grid.is_valid_pos(y as i64, x as i64) {
                match (present.shape[y-row][x-col], grid.at(y, x)) {
                    ('#', GridTile::Free) => continue,
                    ('#', GridTile::Occupied) => {return false;},
                    ('.', _) => continue,
                    _ => panic!("Something went wrong"),
                }
            } else {
                return false;
            }
        }
    }

    for y in row..row+present.row {
        for x in col..col+present.col {
            if present.shape[y-row][x-col] == '#' {
                grid.set_at(y, x, GridTile::Occupied);
            }
        }
    }

    true
}

fn remove_present(grid: &mut Grid<GridTile>, present: &Present, row: usize, col: usize) {
    for y in row..row+present.row {
        for x in col..col+present.col {
            if present.shape[y-row][x-col] == '#' {
                grid.set_at(y, x, GridTile::Free);
            }
        }
    }
}

fn rotate_present_90(present: &Present) -> Present {
    let new_rows = present.col;
    let new_cols = present.row;
    
    let mut rotated_shape = Vec::with_capacity(new_rows);

    for x in 0..present.col {
        let mut row = Vec::with_capacity(new_cols);
        
        for y in (0..present.row).rev() {
            row.push(present.shape[y][x]);
        }
        rotated_shape.push(row);
    }

    Present { 
        row: new_rows, 
        col: new_cols, 
        shape: rotated_shape 
    }
}

fn fill_grid(grid: &mut Grid<GridTile>, presents: &Vec<Present>, quantities: &mut Vec<usize>, num_tries: &mut usize) -> bool { 
    if quantities.iter().all(|x| *x == 0) {
        return true;
    }
    if *num_tries > 5000000 {
        return false;
    }
    let rows = grid.rows();
    let cols = grid.cols();

    for y in 0..rows {
        for x in 0..cols {
            for (idx, present) in presents.iter().enumerate() {
                if quantities[idx] > 0 {
                    let mut to_be_rotated = present.clone();
                    for i in 0..4 {
                        if i == 0 {
                            if add_present(grid, present, y, x) {
                                quantities[idx] -= 1;
                                if fill_grid(grid, presents, quantities, num_tries) {
                                    return true;
                                }
                                quantities[idx] += 1;
                                remove_present(grid, present, y, x);
                            }
                        } else {
                            let rotated = rotate_present_90(&to_be_rotated);
                            if add_present(grid, &rotated, y, x) {
                                quantities[idx] -= 1;
                                if fill_grid(grid, presents, quantities, num_tries ) {
                                    return true;
                                }
                                quantities[idx] += 1;
                                remove_present(grid, &rotated, y, x);
                            }
                            to_be_rotated = rotated;
                        }
                        *num_tries += 1;
                        if *num_tries > 5000000 {
                            return false;
                        }
                    }
                }
            }
        }
    }
    false
}

fn solve_region(reg: &Region, presents: &Vec<Present>) -> bool {
    let mut grid = prepare_grid(reg);
    let mut quantities = reg.quantities.clone();
    fill_grid(&mut grid, presents, &mut quantities, &mut 0)
}

fn main() {
    let (presents, regions) = read_input("input.txt".into());
    let mut g_sum = Arc::new(AtomicUsize::new(0));
    let num_threads = 12;
    let presents = Arc::new(presents);

    std::thread::scope(|s| {
        let chunk_size= regions.len() / num_threads;
        for i in regions.chunks(chunk_size) {
            let presents_for_thread = presents.clone();
            let g_sum_thread = g_sum.clone();
            s.spawn(move || {
                let mut sum = 0;
                let chunk_size = i.len();
                for (idx, region) in i.iter().enumerate() {
                    if solve_region(&region, &presents_for_thread) {
                        sum += 1;
                    }
                    println!("{idx}/{chunk_size}");
                }
                g_sum_thread.fetch_add(sum, std::sync::atomic::Ordering::Release);
            });
        }
    });
    
    println!("Done P1 {}", g_sum.load(std::sync::atomic::Ordering::Relaxed));
}
