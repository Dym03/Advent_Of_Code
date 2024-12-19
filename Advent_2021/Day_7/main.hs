module Main where

import Data.Text (pack, split, unpack)

parseInput :: String -> [Int]
parseInput input = map (read . unpack) $ split (== ',') $ pack input

solveP1 input = minimum [sum $ map (distance x) input | x <- [minimum input .. maximum input]]


solveP2 input = minimum [sum $ map (distance2 x) input | x <- [minimum input .. maximum input]]

distance x y = abs (x - y)

distance2 x y
  | abs (x - y) == 0 = 0
  | otherwise = sum $ take (abs (x - y)) [1 ..]

main = do
  file <- readFile "input.txt"
  print $ solveP1 (parseInput file)
  print $ solveP2 (parseInput file)
