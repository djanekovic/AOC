use std::io::{self, Read};
#[macro_use] extern crate lazy_static;
use regex::Regex;

const REQUIRED_FIELD: [&str; 7] = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];

fn valid_hgt(byr: &str) -> bool {
    if byr.len() == 5 {
        let (height, unit) = byr.split_at(3);
        (150..194).contains(&height.parse::<u32>().unwrap()) && unit == "cm"
    } else if byr.len() == 4 {
        let (height, unit) = byr.split_at(2);
        (59..77).contains(&height.parse::<u32>().unwrap()) && unit == "in"
    } else {
        false
    }
}

fn valid_hcl(hcl: &str) -> bool {
    lazy_static! {
        static ref HCL_REGEX: Regex = Regex::new(r"^#[0-9a-f]{6}$").unwrap();
    }

    if HCL_REGEX.is_match(hcl)
}

fn valid_ecl(ecl: &str) -> bool {
    lazy_static! {
        static ref ECL_REGEX: Regex = Regex::new(r"^(amb|blu|brn|gry|grn|hzl|oth)$").unwrap();
    }

    if ECL_REGEX.is_match(ecl)
}

fn valid_pid(pid: &str) -> bool {
    lazy_static! {
        static ref PID_REGEX: Regex = Regex::new(r"^\d{9}$").unwrap();
    }

    if PID_REGEX.is_match(pid)
}


fn validate_line(s: &str) -> bool {
    let mut required: [bool; 7] = [false; 7];

    for pairs in s.trim().split(|c| c == '\n' || c == ' ') {
        let mut pair_iterator = pairs.split(':');
        let key = pair_iterator.next().unwrap();
        let value = pair_iterator.next().unwrap();
        for (i, f) in REQUIRED_FIELD.iter().enumerate() {
            if *f == key {
                match i {
                    0 => if (1920..2003).contains(&value.parse::<u32>().unwrap()) { required[i] = true; }
                    1 => if (2010..2021).contains(&value.parse::<u32>().unwrap()) { required[i] = true; }
                    2 => if (2020..2031).contains(&value.parse::<u32>().unwrap()) { required[i] = true; }
                    3 => if valid_hgt(value) { required[i] = true; }
                    4 => if valid_hcl(value) { required[i] = true; }
                    5 => if valid_ecl(value) { required[i] = true; }
                    6 => if valid_pid(value) { required[i] = true; }
                    _ => println!("TODO: should panic here or something...")
                }
            }
        }
    }

    required.iter().all(|&x| x)
}

fn part1(input: &String) -> io::Result<usize> {
    let counter = input.split("\n\n").filter(|l| validate_line(l)).count();

    Ok(counter)
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    println!("{}", part1(&input)?);

    Ok(())
}
