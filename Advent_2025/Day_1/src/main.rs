use std::io;

struct Lock {
    dial: Vec<u8>,
    index: usize,
}

impl Lock {
    fn new() -> Lock {
        Lock{ dial: (0..=99).collect(), index: 50 }
    }

    fn shift_left(& mut self, by: usize) -> u32{
        let mut output = 0;
        if self.index == 0 {
            output = (by / 100) as u32;
            self.index = 100 - by % 100;
        } else {
            if by >= self.index {
                let shift_to_zero = self.index;
                
                output += 1;
                output += ((by - shift_to_zero) / 100 ) as u32;

                if by % 100 >= self.index {
                    self.index = (100 - (by % 100 - self.index)) % 100;
                } else {
                    self.index -= by % 100;
                }
            } else {
                self.index -= by % 100;
            }
        }

        output
    }

    fn shift_right (&mut self, by: usize) -> u32 {
        let mut output = 0;
        if (by / (100 - self.index)) as u32 > 0 {
            output = (1 + ((by - (100 - self.index)) / 100)) as u32
        }
        self.index = (self.index + by) % 100;
        output
    }
}

fn main() -> io::Result<()> {
    let mut lock = Lock{ dial: (0..=99).collect(), index: 50 };
    let file_path = "input.txt";
    let file = std::fs::read_to_string(file_path)?;
    let mut p1_num_zeros = 0;
    let mut p2_num_zeros = 0;
    for line in file.lines() {
        let (side, num_spins) = line.split_at(1);
        let by = num_spins.parse::<usize>().expect("Couldn't parse usize");
        match side {
            "L" => { 
                p2_num_zeros += lock.shift_left(by);
             },
            "R" => {
                p2_num_zeros += lock.shift_right(by);
            },
            _ => println!("Unknown Operation")
        }

        if lock.index == 0 {
            p1_num_zeros += 1;
        }
    }

    println!("P1 {p1_num_zeros}");
    println!("P2 {p2_num_zeros}");

    Ok(())
}

#[test]
fn test_1() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_left(51), 1);
    assert_eq!(lock.index, 99);
}

#[test]
fn test_2() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_left(150), 2);
    assert_eq!(lock.index, 0);
}

#[test]
fn test_3() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_left(151), 2);
    assert_eq!(lock.index, 99);
}

#[test]
fn test_7() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_left(139), 1);
    assert_eq!(lock.index, 11);
}

#[test]
fn test_4() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_right(50), 1);
    assert_eq!(lock.index, 0);
}

#[test]
fn test_5() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_right(150), 2);
    assert_eq!(lock.index, 0);
}

#[test]
fn test_6() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_right(151), 2);
    assert_eq!(lock.index, 1);
}

#[test]
fn test_8() {
    let mut lock = Lock::new();
    assert_eq!(lock.shift_left(32), 0);
    assert_eq!(lock.index, 18);
}