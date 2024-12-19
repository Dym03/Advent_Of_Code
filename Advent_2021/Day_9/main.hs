module Main where

import Data.Char (ord)
import Data.List (sortBy)
import Data.Ord (Down (..), comparing)

isInBounds :: Int -> Int -> Int -> Int -> Bool
isInBounds x y maxX maxY = x >= 0 && x < maxX && y >= 0 && y < maxY

solveP1 :: [String] -> Int
solveP1 grid =
  sum $
    [ (\(x, y) -> checkNeighbours x y maxX maxY grid) (x, y)
      | x <- [0 .. maxX - 1],
        y <- [0 .. maxY - 1]
    ]
  where
    maxX = length $ head grid
    maxY = length grid

-- checkNeighbours :: Int -> Int -> Int -> Int -> [[Char]] -> Int
checkNeighbours :: Int -> Int -> Int -> Int -> [String] -> Int
checkNeighbours x y maxX maxY grid = if act_value < minimum neighbourValues then (ord act_value - ord '0') + 1 else 0
  where
    neighbours = [(x, y - 1), (x - 1, y), (x + 1, y), (x, y + 1)]
    validNeighbours = filter (\(x, y) -> isInBounds x y maxX maxY) neighbours
    neighbourValues = map (\(x, y) -> grid !! y !! x) validNeighbours
    act_value = grid !! y !! x

checkNeighboursP2 :: Int -> Int -> Int -> Int -> [String] -> (Int, Int)
checkNeighboursP2 x y maxX maxY grid = if act_value < minimum neighbourValues then (x, y) else (-1, -1)
  where
    neighbours = [(x, y - 1), (x - 1, y), (x + 1, y), (x, y + 1)]
    validNeighbours = filter (\(x, y) -> isInBounds x y maxX maxY) neighbours
    neighbourValues = map (\(x, y) -> grid !! y !! x) validNeighbours
    act_value = grid !! y !! x

solveP2 grid = product . take 3 . sortBy (comparing Data.Ord.Down) $ [bfs [x] [] | x <- startPoints]
  where
    maxX = length $ head grid
    maxY = length grid
    startPoints =
      filter (\(x, y) -> x /= (-1) && y /= (-1)) $
        [ (\(x, y) -> checkNeighboursP2 x y maxX maxY grid) (x, y)
          | x <- [0 .. maxX - 1],
            y <- [0 .. maxY - 1]
        ]
    bfs toBeVisited visited = do
      let (x_act, y_act) = head toBeVisited
      let newTobeVisited = tail toBeVisited
      let neighbours = [(x_act, y_act - 1), (x_act - 1, y_act), (x_act + 1, y_act), (x_act, y_act + 1)]
      let validNeighbours = filter (\(x, y) -> grid !! y !! x /= '9') $ filter (\(x, y) -> isInBounds x y maxX maxY) neighbours
      let newVisited = visited ++ [(x_act, y_act)]
      let newPoints = newTobeVisited ++ filter (\(x, y) -> notElem (x, y) newVisited && notElem (x, y) toBeVisited) validNeighbours
      if null newPoints then length newVisited else bfs newPoints newVisited

main = do
  input <- readFile "input.txt"
  let input' = lines input

  print $ solveP1 input'
  print $ solveP2 input'