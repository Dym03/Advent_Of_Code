use std::{collections::HashMap, io, mem, str::FromStr};

use utils::{Grid, GridTileError};

enum GridTile {
    Start,
    Splitter,
    Empty,
    Beam
}

impl FromStr for GridTile {
    type Err = GridTileError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "S" => Ok(Self::Start),
            "^" => Ok(Self::Splitter),
            "." => Ok(Self::Empty),
            "|" => Ok(Self::Beam),
            _ => Err(GridTileError::UnknownType)
         }
    }
}

impl ToString for GridTile {
    fn to_string(&self) -> String {
        match self {
            GridTile::Start => "S".into(),
            GridTile::Splitter => "^".into(),
            GridTile::Empty => ".".into(),
            GridTile::Beam => "|".into(),
        }
    }
}

fn solve_p1(g: &mut Grid<GridTile>) -> (i32, usize){
    let mut sum = 0;
    let mut runned_beam = false;
    let mut sum_2 = 0;
    let mut memory = HashMap::new();
    for y in 0..g.rows() {
        for x in 0..g.cols() {
            let pos = g.at(y, x);
            match pos {
                GridTile::Start => {
                    runned_beam = true;
                    sum_2 = run_beam(g, x as i64, (y + 1) as i64, &mut memory);
                },
                _ => continue
            }
            if runned_beam {
                break;
            }
        }
        if runned_beam {
            break;
        }
    }

    for y in 0..g.rows() {
        for x in 0..g.cols() {
            let pos = g.at(y, x);
            match pos {
                GridTile::Splitter => {
                    if g.is_valid_pos((y - 1) as i64, x as i64) {
                        match g.at((y-1) as usize, x) {
                            GridTile::Beam => sum += 1,
                            _ => continue
                        }
                    }
                },
                _ => continue
            }
        }
    }
    (sum, sum_2)
}

fn run_beam(g: &mut Grid<GridTile>, x: i64, y: i64, memory: &mut HashMap<(i64, i64), usize>) -> usize {
    if g.is_valid_pos(y, x) {
        if memory.contains_key(&(y, x)) {
            return *memory.get(&(y, x)).unwrap()    
        }
        match g.at(y as usize, x as usize) {
            GridTile::Start => panic!("Shouldn't Happen"),
            GridTile::Splitter => {
                let sum = run_beam(g, x - 1, y, memory) + run_beam(g, x + 1, y, memory);
                memory.insert((y, x), sum);
                sum
            },
            GridTile::Empty => {
                g.set_at(y as usize, x as usize, GridTile::Beam);
                let sum = run_beam(g, x, y + 1, memory);
                memory.insert((y, x), sum);
                sum
            }
            GridTile::Beam => {
                let sum = run_beam(g, x, y + 1, memory);
                memory.insert((y, x), sum);
                sum
            },
        }
    } else {
        memory.insert((y, x), 1);
        1
    }
}

fn main() -> io::Result<()> {
    let mut g: Grid<GridTile> = Grid::new();
    g.load("input.txt".into())?;
    let (p1, p2)= solve_p1(&mut g);
    g.print();
    println!("Part 1 = {p1}");
    println!("Part 2 = {p2}");
    Ok(())
}
