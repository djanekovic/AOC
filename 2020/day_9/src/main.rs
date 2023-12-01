use std::io::{self, Read};
use std::cmp::Ordering;

fn part1(v: &Vec<u64>) -> u64 {
    let mut res: u64 = 0;
    let window_size = 25;
    'outer: for w in v.windows(window_size+1) {
        for i in 0..window_size {
            for j in 0..window_size {
                if w[window_size] == w[i] + w[j] {
                    continue 'outer;
                }
            }
        }
        res = w[window_size];
        break;
    }

    res
}

fn part2(v: &Vec<u64>) -> u64 {
    let sum = part1(v);
    let mut prefix_sum = vec![0; v.len()+1];

    for i in 1..prefix_sum.len() {
        prefix_sum[i] = prefix_sum[i-1] + v[i-1];
    }

    let mut lo = 0;
    let mut hi = 1;

    while hi < prefix_sum.len() {
        match (prefix_sum[hi] - prefix_sum[lo]).cmp(&sum) {
            Ordering::Less => hi += 1,
            Ordering::Greater => lo += 1,
            Ordering::Equal => break,
        }
    }

    let min = v[lo..hi].iter().min().unwrap();
    let max = v[lo..hi].iter().max().unwrap();

    println!("{} {}", min, max);

    return min + max;
}

fn main() -> io::Result<()> {
    let v: Vec<u64>;
    {
        let mut input = String::new();
        let mut stdin = io::stdin();

        stdin.read_to_string(&mut input)?;

        v = input.lines().map(|l| l.parse::<u64>().unwrap()).collect::<Vec<u64>>();
    }

    println!("{}", part1(&v));
    println!("{}", part2(&v));

    Ok(())
}
