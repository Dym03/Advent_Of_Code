module Main where

import Data.Char (isLower)
import Data.Text (pack, splitOn, unpack)

parseLine :: String -> (String, String)
parseLine line = (a', b')
  where
    [a, b] = splitOn (pack "-") (pack line)
    a' = unpack a
    b' = unpack b

parseInput :: String -> [(String, String)]
parseInput input = map parseLine $ lines input

getConnectedCaves :: [(String, String)] -> String -> [String]
getConnectedCaves [] _ = []
getConnectedCaves ((a, b) : xs) cave
  | a == cave = b : getConnectedCaves xs cave
  | b == cave = a : getConnectedCaves xs cave
  | otherwise = getConnectedCaves xs cave

traverseCaves _ [] _ _ = [[]]
traverseCaves connections currentCave visitedCaves small_visited
  | currentCave == "end" = [currentCave : visitedCaves]
  | currentCave == "start" = concat [traverseCaves connections x (currentCave : visitedCaves) small_visited | x <- adjencies]
  | all isLower currentCave && currentCave `elem` visitedCaves && small_visited = []
  | all isLower currentCave && currentCave `elem` visitedCaves && not small_visited = concat [traverseCaves connections x (currentCave : visitedCaves) True | x <- adjencies]
  | otherwise = concat [traverseCaves connections x (currentCave : visitedCaves) small_visited | x <- adjencies]
  where
    adjencies = filter (/= "start") $ getConnectedCaves connections currentCave

main = do
  file <- readFile "test.txt"
  let input = parseInput file
  print input
  print $ length $ traverseCaves input "start" [] True
  print $ length $ traverseCaves input "start" [] False