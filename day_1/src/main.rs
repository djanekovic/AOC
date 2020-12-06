use std::io::{self, Read};

fn main() -> io::Result<()>
{
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    for line in input.lines() {
        let x: i32 = line.trim().parse().expect("Invalid input");
        vec.push(x);
    }

    part1(&vec)?;
    part2(&vec)?;

    Ok(())
}

fn part1(vec: &Vec<i32>) -> io::Result<()>
{
    'outer: for x in vec {
        for y in vec {
            if x + y == 2020 {
                println!("{} * {} = {}", x, y, x*y);
                break 'outer;
            }
        }
    }
    Ok(())
}

fn part2(vec: &Vec<i32>) -> io::Result<()>
{
    'outer: for x in vec {
        for y in vec {
            for z in vec {
                if x + y + z == 2020 {
                    println!("{} * {} * {} = {}", x, y, z, x*y*z);
                    break 'outer;
                }
            }
        }
    }

    Ok(())
}
