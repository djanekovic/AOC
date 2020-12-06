use std::io::{self, Read};

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    let mut v: Vec<u32> = Vec::new();

    for line in input.lines() {
        let (mut lo, mut hi, mut left, mut right) = (0, 127, 0, 7);
        for c in line.chars() {
            match c {
                'F' => hi = (lo+hi)/2,
                'B' => lo = (lo+hi)/2 + 1,
                'R' => left = (left + right)/2 + 1,
                'L' => right = (left+right)/2,
                _ => println!("TODO: panic..."),
            }
        }
        v.push(lo * 8 + left);
    }

    let min_id: u32 = *v.iter().min().unwrap();
    let max_id: u32 = *v.iter().max().unwrap();

    println!("{} {}", min_id, max_id);
    println!("{}", (min_id..max_id+1).sum::<u32>() - v.iter().sum::<u32>());

    Ok(())
}
