module Main where

import Data.Text (pack, split, splitOn, unpack)

simulate :: [Int] -> Int -> [Int]
simulate list 0 = list
simulate list n = simulate (map (\x -> x - 1) $ concatMap checkNew list) (n - 1)
  where
    checkNew x
      | x == 0 = [7, 9]
      | otherwise = [x]

parseInput :: String -> [Int]
parseInput input = map (read . unpack) $ split (== ',') $ pack input

calculateMap :: [(Int, Int)]
calculateMap = [(x, length $ simulate [x] 156) | x <- [0 .. 9]]

solveP2 list x_to_val = sum $ map (\x -> snd (x_to_val !! x)) list

-- This is fast solution not mine >]    
g :: Int -> Int
g = (map g' [0 ..] !!)
  where
    g' 0 = 1
    g' n | n < 9 = 0
    g' n = g (n - 9) + g (n - 7)

f :: Int -> Int
f = (map f' [0 ..] !!)
  where
    f' 0 = 1
    f' n = f (n - 1) + g n

solve :: Int -> [Int] -> Int
solve days = sum . map (\i -> f (days + 8 - i))

part1 :: [Int] -> Int
part1 = solve 80

part2 :: [Int] -> Int
part2 = solve 256

main = do
  file <- readFile "input.txt"
  --   print $ solveP2 (simulate (parseInput file) 100) calculateMap
  print $ part2 $ parseInput file
