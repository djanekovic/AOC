use std::io::{self, Read};
use std::collections::HashMap;
use regex::Regex;

fn parse_mask(mask: &str) -> (u64, u64, u64) {
    let v = mask.split(" = ").last().unwrap();

    let mut ones: u64 = 0;
    let mut floating: u64 = 0;
    let mut zeros: u64 = u64::MAX;
    for (index, c) in v.chars().enumerate() {
        if c == '1' {
            ones = ones | 1<<(35-index);
        } else if c == '0' {
            zeros = zeros ^ 1<<(35-index);
        } else {
            floating |= 1<<(35-index);
        }
    }

    return (ones, zeros, floating);
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();
    stdin.read_to_string(&mut input)?;

    let re = Regex::new(r"mem\[(\d+)\] = (\d+)").unwrap();

    let mut memory: HashMap<u64, u64> = HashMap::new();
    let mut masks: (u64, u64, u64) = (0, 0, 0);
    for line in input.lines() {
        if line.starts_with("mask") {
            masks = parse_mask(line);
        } else {
            let (ones, zeros, floating) = masks;
            let caps = re.captures(line).unwrap();
            let address: u64 = caps[1].parse::<u64>().unwrap() & !floating;
            let value: u64 = caps[2].parse().unwrap();

            // part1
            //let memory_location = memory.entry(address).or_insert(0);
            //*memory_location = (value | ones) & zeros;

            //part2
            let nums: Vec<u64> = (0..64).filter(|i| (floating & (1<<i)) > 0)
                                        .map(|i| 1<<i)
                                        .collect();

            for b in 0..1<<nums.len() {
                let mut subset: u64 = 0;
                for i in 0..nums.len() {
                    if b & (1 << i) > 0 {
                        subset += nums[i];
                    }
                }

                let memory_location = memory.entry(address | ones | subset).or_insert(0);
                *memory_location = value;
            }
        }
    }

    println!("{}", memory.values().sum::<u64>());


    Ok(())
}
