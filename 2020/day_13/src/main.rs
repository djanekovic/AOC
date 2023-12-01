use std::io;
use std::convert::TryFrom;

fn part1(input: &str, mut lower: u32) -> u32 {
    let v: Vec<u32> = input.split(",")
                           .map(|c| match c.parse::<u32>() { Ok(v) => v, Err(_) => 0})
                           .filter(|n| *n != 0)
                           .collect();

    let start_lower = lower;

    loop {
        let m = v.iter().map(|v| lower.wrapping_rem(*v)).position(|x| x == 0);
        match m {
            Some(index) => return (lower - start_lower) * v[index],
            None => lower += 1,
        }
    }
}

fn modpow(x: u64, n: u64, m: u64) -> u64 {
    if n == 0 {
        return 1;
    }

    let mut u: u64 = modpow(x, n/2, m);
    u = (u * u).wrapping_rem(m);

    if n.wrapping_rem(2) == 1 {
        u = (u * x).wrapping_rem(m);
    }

    return u;
}

fn part2(input: &str) -> u64 {
    let mut v: Vec<(u64, u64)> = Vec::new();

    for (index, num) in input.split(",").enumerate() {
        if num.trim() != "x" {
            let bus_id: u64 = num.trim().parse().unwrap();
            let remainder = (bus_id - u64::try_from(index).unwrap().wrapping_rem(bus_id)).wrapping_rem(bus_id);
            v.push((bus_id, remainder));
        }
    }

    let n: u64 = v.iter().map(|t| t.0).product();
    let x: Vec<u64> = v.iter().map(|t| n/t.0).collect();

    //TODO: cleanup
    //let t: u64 = v.iter().zip(x).map(|z| z.0.1 * z.1 * modpow(z.1, z.0.0-2, z.0.0)).sum();
    let mut t = 0;
    for i in 0..x.len() {
        t += v[i].1 * x[i] * modpow(x[i], v[i].0-2, v[i].0);
    }
    t %= n;

    println!(t);

    return 0;
}

fn main() -> io::Result<()> {
    let mut lower = String::new();

    io::stdin().read_line(&mut lower)?;

    let lower: u32 = lower.trim().parse().unwrap();

    let mut input = String::new();
    io::stdin().read_line(&mut input)?;

    println!("{}", part1(&input, lower));

    part2(&input);

    Ok(())
}
