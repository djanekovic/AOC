use std::io::{self, Read};


fn part1(adj_diff: &Vec<u64>) -> usize {
    let one_jolt_cnt = adj_diff.iter().filter(|i| **i == 1).count();
    let three_jolt_cnt = adj_diff.iter().filter(|i| **i == 3).count();

    return one_jolt_cnt * three_jolt_cnt;
}

fn compute_entry(start: u32, end: u32) -> u64 {
    if start == end - 1 || start == end {
        return 1;
    }

    if start >= end {
        return 0;
    }

    return (1..=3).map(|i| compute_entry(start+i, end)).sum();
}

fn part2(adj_diff: &Vec<u64>) -> u64 {
    let mut consecutive_ones_cnt: u32 = 0;
    let mut result: u64 = 1;
    for v in adj_diff {
        if *v == 1 {
            consecutive_ones_cnt += 1;
        } else {
            if consecutive_ones_cnt != 0 {
                result *= compute_entry(0, consecutive_ones_cnt);
                consecutive_ones_cnt = 0;
            }
        }
    }

    return result;
}

fn main() -> io::Result<()> {
    let mut v = vec![0];
    {
        let mut input = String::new();
        let mut stdin = io::stdin();

        stdin.read_to_string(&mut input)?;

        v.extend(input.lines().map(|l| l.parse::<u64>().unwrap()).collect::<Vec<u64>>());
    }

    v.push(v.iter().max().unwrap() + 3);

    v.sort();
    let adj_diff = &v.windows(2).map(|w| w[1] - w[0]).collect::<Vec<_>>();

    println!("{}", part1(&adj_diff));
    println!("{}", part2(&adj_diff));

    Ok(())
}

