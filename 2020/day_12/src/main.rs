use std::io::{self, Read};
use regex::Regex;

#[derive(Debug, Clone, Copy)]
enum Direction {
    East,
    North,
    West,
    South,
    Left,
    Right,
    Forward,
}

impl Direction {
    fn from_u8(value: u8) -> Direction {
        match value {
            0 => Direction::East,
            1 => Direction::North,
            2 => Direction::West,
            3 => Direction::South,
            4 => Direction::Left,
            5 => Direction::Right,
            6 => Direction::Forward,
            _ => panic!("Unsupported value {}", value),
        }
    }
}

#[derive(Debug, Copy, Clone)]
struct Location {
    x: i64,
    y: i64,
}


#[derive(Debug)]
struct Command {
    direction: Direction,
    value: i64,
}

fn parse_direction(direction: &str) -> Direction {
    let ch = direction.chars().next().unwrap();

    match ch {
        'N' => Direction::North,
        'S' => Direction::South,
        'E' => Direction::East,
        'W' => Direction::West,
        'L' => Direction::Left,
        'R' => Direction::Right,
        'F' => Direction::Forward,
        _   => panic!("Unknown direction {}", ch),
    }
}

fn move_ship(location: &Location, direction: Direction, value: i64) -> Location {
    let mut new_location = location.clone();

    match direction {
        Direction::North => new_location.y = location.y + value,
        Direction::South => new_location.y = location.y - value,
        Direction::East => new_location.x = location.x + value,
        Direction::West => new_location.x = location.x - value,
        _ => panic!("Unknown direction {:?}", direction),
    }

    return new_location;
}

fn rotate_ship(direction: Direction, value: i64) -> Direction {
    match value {
        90 => Direction::from_u8((direction as u8 + 1).wrapping_rem(4)),
        180 => Direction::from_u8((direction as u8 + 2).wrapping_rem(4)),
        270 => Direction::from_u8((direction as u8 + 3).wrapping_rem(4)),
        -90 => Direction::from_u8((direction as u8 + 3).wrapping_rem(4)),
        -180 => Direction::from_u8((direction as u8 + 2).wrapping_rem(4)),
        -270 => Direction::from_u8((direction as u8 + 1).wrapping_rem(4)),
        _ => panic!("Unknown angle for rotation {}", value),
    }
}

fn part1(v: &Vec<Command>) -> i64 {
    let mut cur_direction = Direction::East;
    let mut location = Location{x: 0, y: 0};

    for command in v {
        match command.direction {
            Direction::Left => cur_direction = rotate_ship(cur_direction, command.value),
            Direction::Right => cur_direction = rotate_ship(cur_direction, -command.value),
            Direction::Forward => location = move_ship(&location, cur_direction, command.value),
            _ => location = move_ship(&location, command.direction, command.value),
        }
    }

    return location.x.abs() + location.y.abs();
}

fn rotate_waypoint(waypoint: &Location, value: i64) -> Location {
    match value {
        90 |-270 => Location{x: -waypoint.y, y: waypoint.x},
        180 |-180 => Location{x: -waypoint.x, y: -waypoint.y},
        270 | -90 => Location{x: waypoint.y, y: -waypoint.x},
        _ => panic!("Unknown angle for rotation {}", value),
    }
}


fn part2(v: &Vec<Command>) -> i64 {
    let mut waypoint = Location{x: 10, y: 1};
    let mut ship_location = Location{x: 0, y: 0};

    for command in v {
        match command.direction {
            Direction::Left => waypoint = rotate_waypoint(&waypoint, command.value),
            Direction::Right => waypoint = rotate_waypoint(&waypoint, -command.value),
            Direction::North => waypoint.y += command.value,
            Direction::South => waypoint.y -= command.value,
            Direction::East => waypoint.x += command.value,
            Direction::West => waypoint.x -= command.value,
            Direction::Forward => {
                ship_location.x += waypoint.x * command.value;
                ship_location.y += waypoint.y * command.value;
            }
        }
    }

    return ship_location.x.abs() + ship_location.y.abs();
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    let mut stdin = io::stdin();

    stdin.read_to_string(&mut input)?;

    let re = Regex::new(r"([NSEWLRF])(\d+)").unwrap();
    let v: Vec<Command> = re.captures_iter(&input)
                            .map(|c| -> Command {
                                let direction = parse_direction(&c[1]);
                                let value: i64 = c[2].parse().unwrap();
                                Command {direction: direction, value: value}})
                            .collect();

    println!("{}", part1(&v));
    println!("{}", part2(&v));

    Ok(())
}
