module Main where

import Data.Char (ord)
import Data.List (nub)

incrementGrid :: [[Int]] -> [[Int]]
incrementGrid = map (map (+1))

isInBounds :: Int -> Int -> Int -> Int -> Bool
isInBounds x y xMax yMax = x >= 0 && x <= xMax && y >= 0 && y <= yMax

getNeighbors :: Int -> Int -> Int -> Int -> [(Int, Int)]
getNeighbors x y xMax yMax =
  [(nx, ny) | nx <- [x-1..x+1], ny <- [y-1..y+1], (nx, ny) /= (x, y), isInBounds nx ny xMax yMax]

resetFlashed :: [[Int]] -> [(Int, Int)] -> [[Int]]
resetFlashed grid flashed =
  [[if (x, y) `elem` flashed then 0 else grid !! y !! x | x <- [0..xMax]] | y <- [0..yMax]]
  where
    xMax = length (head grid) - 1
    yMax = length grid - 1

simulateFlashes :: [[Int]] -> [(Int, Int)] -> [[Int]]
simulateFlashes grid flashed =
  let
    xMax = length (head grid) - 1
    yMax = length grid - 1
    toFlash = [(x, y) | x <- [0..xMax], y <- [0..yMax], grid !! y !! x > 9, (x, y) `notElem` flashed]
    neighbors = concatMap (\(x, y) -> getNeighbors x y xMax yMax) toFlash
    updatedGrid = foldl incrementCell grid neighbors
    flashedNow = flashed ++ toFlash
  in
    if null toFlash
      then resetFlashed updatedGrid flashedNow
      else simulateFlashes updatedGrid flashedNow

incrementCell :: [[Int]] -> (Int, Int) -> [[Int]]
incrementCell grid (x, y) =
  let current = grid !! y !! x
  in updateGrid grid (x, y) (current + 1)

updateGrid :: [[a]] -> (Int, Int) -> a -> [[a]]
updateGrid grid (x, y) value =
  take y grid ++
  [take x (grid !! y) ++ [value] ++ drop (x + 1) (grid !! y)] ++
  drop (y + 1) grid

numOfZeros :: [[Int]] -> Int
numOfZeros = length . filter (== 0) . concat

solveP1 :: Int -> [[Int]] -> Int
solveP1 steps grid =
  let simulations = take steps $ iterate (simulateFlashes . incrementGrid) grid
  in sum $ map numOfZeros simulations

solveP2 :: [[Int]] -> Int
solveP2 grid =
  let
    simulations = iterate (simulateFlashes . incrementGrid) grid
    syncStep = length grid * length (head grid)
  in
    length $ takeWhile (\g -> numOfZeros g /= syncStep) simulations

parseInput :: String -> [[Int]]
parseInput = map (map (read . (:[]))) . lines

main :: IO ()
main = do
  file <- readFile "input.txt"
  let input = parseInput file
  print $ solveP1 100 input
  print $ solveP2 input
