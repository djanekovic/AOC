use std::io::{self, Read};
#[macro_use] extern crate lazy_static;
use regex::Regex;

#[derive(Default, Debug)]
struct PasswordPolicy {
    low: usize,
    high: usize,
    letter: char,
    passwd: String,
}

fn parse_policy(line: &str) -> io::Result<PasswordPolicy> {
    lazy_static! {
        static ref RE: Regex = Regex::new(r"([\d]+)-([\d]+) ([a-z]): ([\w]+)").unwrap();
    }

    let mut policy: PasswordPolicy = Default::default();

    for cap in RE.captures_iter(&line) {
        policy.low = cap[1].parse().expect("Wrong format");
        policy.high = cap[2].parse().expect("Wrong format");
        policy.letter = cap[3].chars().next().expect("String is empty");
        //TODO: this is a bug because we are doing extra alloc we don't need
        policy.passwd = cap[4].to_string();
    }

    Ok(policy)
}

fn part1(v: &Vec<PasswordPolicy>) -> io::Result<()> {
    let valid_policy = v.iter()
                        .filter(|p| -> bool {
                            let a = p.low..p.high+1;
                            a.contains(&p.passwd.chars().filter(|c| *c == p.letter).count())
                        }).count();

    println!("{}", valid_policy);

    Ok(())
}

fn part2(v: &Vec<PasswordPolicy>) -> io::Result<()> {
    let valid_policy = v.iter()
                        .filter(|p| -> bool {
                            (p.passwd.chars().nth(p.low-1).unwrap() == p.letter) ^
                            (p.passwd.chars().nth(p.high-1).unwrap() == p.letter)
                        }).count();

    println!("{}", valid_policy);
    Ok(())
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    let mut v: Vec<PasswordPolicy> = Vec::new();
    for line in input.lines() {
        v.push(parse_policy(&line)?);
    }

    part1(&v)?;
    part2(&v)?;

    Ok(())
}
