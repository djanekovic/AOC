use std::io::{self, Read};
use std::collections::HashSet;
use std::collections::HashMap;

fn answers_any(group: &str) -> usize {
    let mut questions = HashSet::new();
    for person in group.lines() {
        for c in person.chars() {
            questions.insert(c);
        }
    }

    questions.len()
}

fn answers_all(group: &str) -> usize {
    let mut questions = HashMap::new();
    for person in group.lines() {
        for c in person.chars() {
            let counter = questions.entry(c).or_insert(0);
            *counter += 1;
        }
    }

    questions.values().filter(|v| **v == group.lines().count()).count()
}

fn part1(input: &str) -> io::Result<usize> {
    let counter = input.split("\n\n").map(|g| answers_any(g)).sum::<usize>();

    Ok(counter)
}

fn part2(input: &str) -> io::Result<usize> {
    let counter = input.split("\n\n").map(|g| answers_all(g)).sum::<usize>();

    Ok(counter)
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    println!("{}", part1(&input)?);
    println!("{}", part2(&input)?);

    Ok(())
}
