module Main where

import Data.List (sort)
import Data.Map (Map, fromListWith, toList)
import Data.Text (pack, split, splitOn, unpack)

data Range = Range {x1 :: Int, y1 :: Int, x2 :: Int, y2 :: Int} deriving (Show)

-- parseLine :: String -> [Range]
parseLine :: String -> [Int]
parseLine line = map (read . unpack) $ concatMap (splitOn (pack ",") . pack) $ filter (/= "->") $ words line

parseInputP1 input = filter (\r -> x1 r == x2 r || y1 r == y2 r) $ map (extractRange . parseLine) (lines input)

parseInputP2 input = map (extractRange . parseLine) (lines input)

extractRange :: [Int] -> Range
extractRange [] = Range 0 0 0 0
extractRange [x1, y1, x2, y2] = Range x1 y1 x2 y2

isLine :: Range -> Bool
isLine range = x1 range == x2 range || y1 range == y2 range

generateAllPoint :: Range -> [(Int, Int)]
generateAllPoint (Range x1 y1 x2 y2)
  | isLine Range {x1, y1, x2, y2} = [(x, y) | x <- [min x2 x1 .. max x2 x1], y <- [min y2 y1 .. max y2 y1]]
  | otherwise = [(x, y) | x <- [min x2 x1 .. max x2 x1], y <- [min y2 y1 .. max y2 y1], x - y == x1 - y1 || x + y == x1 + y1]

generateMap :: [Range] -> Map (Int, Int) Int
generateMap ranges = fromListWith (+) [(point, 1) | point <- concatMap generateAllPoint ranges]

solveP1 ranges = length $ filter (\(_, c) -> c > 1) $ toList $ generateMap ranges

main = do
  input <- readFile "input.txt"
  print $ solveP1 $ parseInputP1 input
  print $ solveP1 $ parseInputP2 input