use core::{f64, num};
use std::{collections::{HashMap, HashSet}, io, path::PathBuf};

#[derive(Debug, Hash, Clone, Copy, PartialEq, Eq)]
struct Vec3 {
    x: i64,
    y: i64,
    z: i64
}

fn read_file(path: PathBuf) -> io::Result<Vec<Vec3>> {
    let file = std::fs::read_to_string(path)?;
    let mut positions = vec![];
    for line in file.lines() {
        let mut pos = Vec3 { x: 0, y: 0, z: 0 };
        let mut it = line.split(",");
        pos.x = it.next().unwrap().parse::<i64>().expect("Unable to parse i64");
        pos.y = it.next().unwrap().parse::<i64>().expect("Unable to parse i64");
        pos.z = it.next().unwrap().parse::<i64>().expect("Unable to parse i64");
        positions.push(pos);
    }

    Ok(positions)
}

fn euklidian_dist(this: &Vec3, other: &Vec3) -> f64 {
    (((this.x - other.x).pow(2) + (this.y - other.y).pow(2) + (this.z - other.z).pow(2)) as f64).sqrt()
}

fn solve_p1(circuits:&mut HashMap<usize, Vec<Vec3>>, num_cycles: usize) { 
    let mut set: HashSet<(Vec3, Vec3)> = HashSet::new();
    loop {
    // for i in 0..num_cycles { // P1 
        let mut min_dist = f64::MAX;
        let mut to_be_removed = 0;
        let mut to_be_assigned_to = 0;
        let mut to_be_assigned = vec![];
        let mut p_1 = None;
        let mut p_2 = None;
        let mut same_cluster = false;
        for (idx, (key, positions)) in circuits.iter().enumerate() {
            for (idx_2, (key_2, posisitions_2)) in circuits.iter().enumerate() {
                if idx_2 <= idx {
                    continue;
                }
                // For P1
                // for (idx, pos) in positions.iter().enumerate() {
                //     for pos_2 in positions[idx + 1 .. ].iter() {
                //         if set.contains(&(*pos, *pos_2)) {
                //             continue;
                //         }
                //         let dist = euklidian_dist(pos, pos_2);
                //         if dist < min_dist {
                //             min_dist = dist;
                //             p_1 = Some(pos);
                //             p_2 = Some(pos_2);
                //             same_cluster = true;
                //         }
                //     }
                // }

                for pos in positions.iter() {
                    for pos_2 in posisitions_2.iter() {
                        if set.contains(&(*pos, *pos_2)) {
                            continue;
                        }
                        let dist = euklidian_dist(pos, pos_2);
                        if dist < min_dist {
                            to_be_removed = *key_2;
                            to_be_assigned_to = *key;
                            to_be_assigned = posisitions_2.clone();
                            min_dist = dist;
                            p_1 = Some(pos);
                            p_2 = Some(pos_2);
                            same_cluster = false;
                        }
                    }
                }
                
            }
        }

        let p1 = p_1.unwrap().clone();
        let p2 = p_2.unwrap().clone();
        set.insert((p1, p2));
        // if same_cluster { // P1 
        //     continue;
        // }

        let mut to_be_appended = circuits.remove(&to_be_removed).unwrap();
        circuits.get_mut(&to_be_assigned_to).unwrap().append(&mut to_be_appended);
        if circuits.len() == 1 {
            println!("{p1:?} {p2:?}");
            println!("P2 {}", p1.x * p2.x);
            break;
        }

    }

    let mut c: Vec<&Vec<Vec3>> = circuits.iter().map(|(key, val)| val).collect();

    c.sort_by(|x, y| x.len().cmp(&y.len()));

    let mut sum = 1;
    for cluster in c.iter().rev().take(3) {
        println!("Size {} {cluster:?}", cluster.len());
        sum *= cluster.len();
    }
    println!("P1 {sum}");


}

fn to_map(junktion_boxes: Vec<Vec3>) -> HashMap<usize, Vec<Vec3>> {
    let mut circuits = HashMap::new();
    for (idx, pos) in junktion_boxes.iter().enumerate() {
        circuits.insert(idx, vec![*pos]);
    }

    circuits
}

fn main() -> io::Result<()> {
    let junktion_boxes = read_file("input.txt".into())?;
    // solve_p1_2(&junktion_boxes, 10);
    let mut circuits = to_map(junktion_boxes);
    solve_p1(&mut circuits, 1000);

    Ok(())
}
