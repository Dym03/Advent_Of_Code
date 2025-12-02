module Main where

import Data.Char (ord)
import Data.Set (fromList)

-- The same as [map (1 +) x | x <- grid]
stepPart1 = map (map (1 +))

isValid x y xMax yMax = x >= 0 && x < xMax && y >= 0 && y < yMax

getNeighbors x y xMax yMax = filter (\(x, y) -> isValid x y xMax yMax) [(x_1, y_1) | x_1 <- [(x - 1) .. (x + 1)], y_1 <- [(y - 1) .. (y + 1)], x /= x_1 || y /= y_1]

flashGrid grid = neighbours
  where
    xMax = length (head grid) - 1
    yMax = length grid - 1
    locations = [(x, y) | x <- [0 .. xMax], y <- [0 .. yMax]]
    flashLocation = filter (\(x, y) -> grid !! y !! x > 9) locations
    neighbours = fromList [getNeighbors x y xMax yMax | (x, y) <- flashLocation]

resetFlashed grid flashed = [[if (x, y) `elem` flashed then 0 else grid !! y !! x | x <- [0 .. xMax]] | y <- [0 .. yMax]]
  where
    xMax = length (head grid) - 1
    yMax = length grid - 1

flash grid flashed =
  let xMax = length (head grid)
      yMax = length grid
      flashLocation = [(x, y) | x <- [0 .. (xMax - 1)], y <- [0 .. (yMax - 1)], grid !! y !! x > 9 && notElem (x, y) flashed]
      flashedNow = flashed ++ flashLocation
      neighbours = concat [getNeighbors x y xMax yMax | (x, y) <- flashLocation]
      newGrid = [[(grid !! y !! x) + length (filter (\(x_n, y_n) -> x_n == x && y_n == y) neighbours) | x <- [0 .. (xMax - 1)]] | y <- [0 .. (yMax - 1)]]
   in if null neighbours
        then resetFlashed newGrid flashedNow
        else flash newGrid flashedNow

numOfZeros :: [[Int]] -> Int
numOfZeros = length . filter (== 0) . concat

solveP1 :: Int -> [[Int]] -> Int -> Int
solveP1 0 grid counter = counter
solveP1 n grid counter = solveP1 (n - 1) newGrid (counter + numOfZeros newGrid)
  where
    newGrid = flash (stepPart1 grid) []

solveP2 n grid = do
  let newGrid = flash (stepPart1 grid) []
  let zeros = numOfZeros newGrid
  if zeros /= (length grid * length (head grid)) then solveP2 (n + 1) newGrid else n

parseInput file = map parseLine $ (lines file)
  where
    parseLine :: [Char] -> [Int]
    parseLine [] = []
    parseLine (x : xs) = (ord x - ord '0') : parseLine xs

gridAsString grid = [concat [show x | x <- line] ++ "\n" | line <- grid]

showGrid :: [[Int]] -> String
showGrid = unlines . map (unwords . map show)

main = do
  file <- readFile "input.txt"
  let input = parseInput file
  print $ "Part 1 = " ++ (show $ solveP1 100 input 0)
  print $ "Part 2 = " ++ (show $ solveP2 1 input)