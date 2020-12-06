use std::io::{self, Read};

fn count_tree(v: &Vec<&str>, step_down: usize, step_right: usize) -> io::Result<usize> {
    let mut curr_pos = 0;

    let counter = v.iter()
                   .skip(step_down)
                   .step_by(step_down)
                   .filter(|l| -> bool {
                       curr_pos = (curr_pos + step_right) % l.len();
                       l.chars().nth(curr_pos).unwrap() == '#'
                   })
                   .count();
    Ok(counter)
}

fn part1(v: &Vec<&str>) -> io::Result<usize> {
    Ok(count_tree(v, 1, 3)?)
}

fn part2(v: &Vec<&str>) -> io::Result<usize> {
    let res = [count_tree(v, 1, 1)?,
               count_tree(v, 1, 3)?,
               count_tree(v, 1, 5)?,
               count_tree(v, 1, 7)?,
               count_tree(v, 2, 1)?];

    Ok(res.iter().fold(1, |acc, x| acc * x))
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    let mut v: Vec<&str> = Vec::new();

    for line in input.lines() {
        v.push(line);
    }

    println!("{}", part1(&v)?);
    println!("{}", part2(&v)?);


    Ok(())
}
