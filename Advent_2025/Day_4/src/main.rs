use std::{io::{self, Write}, path::PathBuf, str::FromStr};

#[derive(Debug, PartialEq)]
enum GridTile {
    PaperRoll,
    Empty
}

enum GridTileError {
    UnknownType
}

impl FromStr for GridTile {
    type Err = GridTileError;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "@" => Ok(GridTile::PaperRoll),
            "." => Ok(GridTile::Empty),
            _ => Err(GridTileError::UnknownType)
        }
    }
}

impl ToString for GridTile {
    fn to_string(&self) -> String {
        match self {
            GridTile::PaperRoll => "@".to_string(),
            GridTile::Empty => ".".to_string(),
        }
    }
}

enum NeigboursType {
    Adjacent4,
    Adjacent8
}

struct Grid<T> {
    grid: Vec<Vec<T>>
}

impl<T> Grid <T> {
    fn new() -> Grid<T> {
        Grid { grid: vec![] }
    }

    fn at(&self, row: usize, col: usize) -> &T {
        &self.grid[row][col]
    }

    fn set_at(&mut self, row: usize, col: usize, val: T) {
        self.grid[row][col] = val
    }
    
    fn add_row(&mut self, row: Vec<T>) {
        self.grid.push(row);
    }

    fn is_valid_pos(&self, row: i64, col: i64) -> bool{
        row >= 0 && row < self.rows() as i64 && col >= 0 && col < self.cols() as i64
    }

    fn rows(&self) -> usize {
        self.grid.len()
    }

    fn cols(&self) -> usize {
        self.grid.first().unwrap().len()
    }

    fn neighbours(&self, row: usize, col: usize, which: NeigboursType) -> Vec<&T> {
        let mut adjacent = vec![];
        match which {
            NeigboursType::Adjacent4 => {
                let adj_positions = [(1, 0), (-1, 0), (0, 1), (0, -1)];
                for (dx, dy) in adj_positions {
                    let (new_row, new_col) = (row as i64 + dy, col as i64 + dx); 
                    if self.is_valid_pos(new_row, new_col) {
                        adjacent.push(self.at(new_row as usize, new_col as usize));
                    }
                }
            },
            NeigboursType::Adjacent8 => {
                for dy in -1..=1 {
                    for dx in -1..=1 {
                        if dx == 0 && dy == 0 {
                            continue;
                        }
                        let (new_row, new_col) = (row as i64 + dy, col as i64 + dx); 
                        if self.is_valid_pos(new_row, new_col) {
                            adjacent.push(self.at(new_row as usize, new_col as usize));
                        }   
                    } 
                }
            },
        }

        adjacent
    }
}

impl<T: FromStr> Grid<T> {
    fn load(&mut self, path: PathBuf) -> io::Result<()> {
        let file = std::fs::read_to_string(path)?;

        for line in file.lines() {
            let mut row = vec![];
            for char in line.chars() {
                let s = char.to_string();
                if let Ok(tile) = T::from_str(&s) {
                    row.push(tile);
                } else {
                    println!("Unable to parse Gridtile from {char}");
                }
            }
            self.add_row(row);
        }

        Ok(())
    }
}

impl<T: ToString> Grid<T> {
    fn print(&self) {
        for y in 0..self.rows() {
            for x in 0..self.cols() {
                print!("{}", self.at(y, x).to_string());
            }
            println!("");
        }
    }
}

fn solve_p1(grid: &Grid<GridTile>) -> usize {
    let mut can_be_accessed = 0;
    for y in 0..grid.rows() {
        for x in 0..grid.cols() {
            if *grid.at(y, x) == GridTile::PaperRoll {
                let neighbours = grid.neighbours(y, x, NeigboursType::Adjacent8);
                let filtered: Vec<&&GridTile> = neighbours.iter().filter(|x| match x {
                    GridTile::PaperRoll => true,
                    GridTile::Empty => false,
                }).collect();
                if filtered.len() < 4 {
                    can_be_accessed += 1;
                }
            }
        }
    }
    can_be_accessed
}


fn solve_p2(mut grid: Grid<GridTile>) -> usize {
    let mut can_be_accessed = 0;
    loop {
        let mut to_be_removed = vec![];
        for y in 0..grid.rows() {
            for x in 0..grid.cols() {
                if *grid.at(y, x) == GridTile::PaperRoll {
                    let neighbours = grid.neighbours(y, x, NeigboursType::Adjacent8);
                    let filtered: Vec<&&GridTile> = neighbours.iter().filter(|x| match x {
                        GridTile::PaperRoll => true,
                    GridTile::Empty => false,
                }).collect();
                if filtered.len() < 4 {
                    can_be_accessed += 1;
                    to_be_removed.push((y, x));
                }
            }
            }
        }
        if to_be_removed.is_empty() {
            break;
        }
        for (y, x) in to_be_removed {
            grid.set_at(y, x, GridTile::Empty);
        }
    }
    can_be_accessed
}

fn main() -> io::Result<()>{
    let mut grid = Grid::new();
    grid.load("input.txt".into())?;
    // grid.print();
    println!("P1 = {}", solve_p1(&grid));
    println!("P1 = {}", solve_p2(grid));
    Ok(())
}
