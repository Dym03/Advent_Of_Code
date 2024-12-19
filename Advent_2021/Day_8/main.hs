module Main where

import Data.List (sort, sortOn)
import Data.Set (fromList, member, union)

parseInput input = map words (lines input)

parseLine :: [String] -> ([String], [String])
parseLine (x : xs) = (takeWhile (/= "|") (x : xs), drop 1 (dropWhile (/= "|") (x : xs)))

-- let (rules, messages) = unzip input
--   print rules
--   print messages

solveP1 input = length $ concat [filter (\a -> length a `elem` [2, 3, 4, 7]) (snd x) | x <- input]

solveP2 [] = []
solveP2 ((x, y) : xs) = getCode [toNumbers a (sortOn length x) | a <- y] : solveP2 xs

getCode :: [Int] -> Int
getCode [] = 0
getCode (x1 : x2 : x3 : x4 : _) = fromIntegral (x1 * 10 ^ 3 + x2 * 10 ^ 2 + x3 * 10 + x4)

-- rules = [sortOn length (fst x) | x <- input]

toNumbers x rules
  | length x == 2 = 1
  | length x == 4 = 4
  | length x == 3 = 7
  | length x == 7 = 8
  | length (filter (\a -> not $ member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 1 && length (filter (\a -> member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 5 = 9
  | length (filter (\a -> not $ member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 1 && length (filter (\a -> member a (fromList (head rules) `union` fromList (rules !! 1))) x) == 3 = 3
  | length x == 5 && length (filter (\a -> not $ member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 2 = 2
  | length x == 5 && length (filter (\a -> not $ member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 1 = 5
  | length x == 6 && length (filter (\a -> not $ member a ((fromList (head rules) `union` fromList (rules !! 1)) `union` fromList (rules !! 2))) x) == 2 && length (filter (\a -> member a (fromList (head rules))) x) == 1 = 6
  | otherwise = 0

main = do
  file <- readFile "input.txt"
  let input = parseInput file
  let parsedInput = map parseLine input
  print $ solveP1 parsedInput
  print $ sum $ solveP2 parsedInput