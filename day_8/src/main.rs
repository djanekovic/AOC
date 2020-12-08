use std::io::{self, Read};
use std::convert::TryFrom;
use std::convert::TryInto;

#[derive(Debug, Clone)]
enum InstructionOp {
    Acc,
    Jmp,
    Nop,
}

#[derive(Debug, Clone)]
struct Instruction {
    op: InstructionOp,
    argument: i32,
}

fn parse_line(s: &str) -> Instruction {
    let v: Vec<&str> = s.split(" ").collect();
    let argument = v[1].parse::<i32>().unwrap();
    match v[0] {
        "acc" => Instruction{ op: InstructionOp::Acc, argument: argument },
        "jmp" => Instruction{ op: InstructionOp::Jmp, argument: argument },
        "nop" => Instruction{ op: InstructionOp::Nop, argument: argument },
        _ => panic!("Unknown instruction: {}", v[0]),
    }
}

fn find_loop(program: &Vec<Instruction>) -> (bool, i64) {
    let mut pc: usize = 0;
    let mut acc: i64 = 0;

    let mut visited_instructions = vec![false; program.len()];
    while pc < program.len() {
        if visited_instructions[pc] {
            return (false, acc);
        } else {
            visited_instructions[pc] = true;
        }

        let instruction = &program[pc];

        let mut tmp_pc: i32 = pc.try_into().unwrap();
        match instruction.op {
            InstructionOp::Acc => acc += i64::try_from(instruction.argument).unwrap(),
            InstructionOp::Jmp => tmp_pc += i32::try_from(instruction.argument - 1).unwrap(),
            InstructionOp::Nop => (),
        }
        tmp_pc += 1;
        pc = usize::try_from(tmp_pc).unwrap();
    }

    return (true, acc);
}

fn part1(program: &Vec<Instruction>) -> i64 {
    let (_, v) = find_loop(program);

    v
}

fn part2(program: Vec<Instruction>) -> i64 {
    for (index, instruction) in program.iter().enumerate() {
        let mut alt_program = program.clone();

        match instruction.op {
            InstructionOp::Jmp => alt_program[index].op = InstructionOp::Nop,
            InstructionOp::Nop => alt_program[index].op = InstructionOp::Jmp,
            InstructionOp::Acc => continue,
        }

        let (program_fixed, acc_value) = find_loop(&alt_program);

        if program_fixed {
            return acc_value;
        }
    }

    return 0;
}

fn main() -> io::Result<()> {
    let program: Vec<Instruction>;
    {
        let mut input = String::new();
        let mut stdin = io::stdin();

        stdin.read_to_string(&mut input)?;

        program = input.lines().map(|l| parse_line(l)).collect::<Vec<Instruction>>();
    }

    println!("{}", part1(&program));
    println!("{}", part2(program));

    Ok(())

}
