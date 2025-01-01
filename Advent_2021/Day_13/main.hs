module Main where

import Data.List (find, sort)
import Data.Set (fromList)

splitOn :: (Eq a) => a -> [a] -> [[a]]
splitOn _ [] = []
splitOn delim str =
  let (firstline, remainder) = break (== delim) str
   in firstline : splitOn delim (drop 1 remainder)

makeGrid coords =
  let x = maximum (map fst coords)
      y = maximum (map snd coords)
   in [[if (a, b) `elem` coords then "#" else "." | a <- [0 .. x]] | b <- [0 .. y]]

parseTuple str = (read (takeWhile (/= ',') str) :: Int, read (tail (dropWhile (/= ',') str)) :: Int)

parseFold str = (filter (\x -> x == 'x' || x == 'y') str, read $ tail (dropWhile (/= '=') str) :: Int)

applyFold (axis, amount) (x, y)
  | axis == "x" && amount > x = (x, y)
  | axis == "x" && amount < x = (amount - (x - amount), y)
  | axis == "y" && amount > y = (x, y)
  | axis == "y" && amount < y = (x, amount - (y - amount))
  | otherwise = (-1, -1)

solvePart1 coords [] = coords
solvePart1 coords folds =
  let new_coords = map (applyFold (head folds)) coords
      new_folds = tail folds
   in solvePart1 new_coords new_folds

main = do
  input <- readFile "input.txt"
  let l_input = splitOn "" (lines input)
  let coords_tuples = map parseTuple (head l_input)
  let folds = map parseFold (l_input !! 1)
  let grid = makeGrid
  print $ length $ fromList $ map (applyFold (head folds)) coords_tuples
  let result = solvePart1 coords_tuples folds
  putStrLn $ concat $ [concat (x ++ ["\n"]) | x <- makeGrid result]