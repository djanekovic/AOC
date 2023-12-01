use std::io::{self, Read};
use std::collections::HashMap;
use std::collections::VecDeque;

#[derive(Debug)]
struct Bag {
    number: u64,
    color: String,
}

fn bfs(tree: &HashMap<String, Vec<Bag>>, start: &str, end: &str) -> bool {
    if start == end {
        return false
    }

    let mut q: VecDeque<&str> = VecDeque::new();
    q.push_back(start);

    while !q.is_empty() {
        let p = q.pop_front().unwrap();

        if p == end {
            return true
        }

        for n in tree.get(p).unwrap().iter() {
            q.push_back(&n.color);
        }
    }
    false
}

fn dfs(tree: &HashMap<String, Vec<Bag>>, start: &str) -> u64 {
    let mut sum: u64 = 0;
    for bag in tree.get(start).unwrap().iter() {
        sum += bag.number + bag.number * dfs(tree, &bag.color)
    }

    sum
}

fn part1(tree: &HashMap::<String, Vec<Bag>>) {
    println!("{}", tree.iter().filter(|(k, _)| bfs(tree, k, "shiny gold")).count());
}

fn part2(tree: &HashMap::<String, Vec<Bag>>) {
    println!("{}", dfs(tree, "shiny gold"));
}

fn parse_root(root: &str) -> (String, Vec<Bag>) {
    let v: Vec<&str> = root.split(" ").collect();
    let mut leafs: Vec<Bag> = Vec::new();

    let root_name = [v[0], v[1]].join(" ");
    if v[4] != "no" {
        let leaf_name = [v[5], v[6]].join(" ");
        let number: u64 = v[4].parse().unwrap();
        leafs.push(Bag{number: number, color: leaf_name});
    }


    (root_name, leafs)
}

fn parse_leaf(leaf: &str, leafs: &mut Vec<Bag>) {
    let v: Vec<&str> = leaf.trim().split(" ").collect();

    let number: u64 = v[0].parse().unwrap();
    let leaf_name = [v[1], v[2]].join(" ");

    leafs.push(Bag{number: number, color: leaf_name});
}

fn main() -> io::Result<()> {
    let mut tree = HashMap::<String, Vec<Bag>>::new();
    {
        let mut input = String::new();
        let mut stdin = io::stdin();

        stdin.read_to_string(&mut input)?;

        for line in input.lines() {
            let v: Vec<&str> = line.split(",").collect();
            let (root, mut leafs) = parse_root(v[0]);
            for leaf in v.iter().skip(1) {
                parse_leaf(leaf, &mut leafs);
            }

            tree.insert(root, leafs);
        }

    }

    part1(&tree);
    part2(&tree);

    Ok(())
}
