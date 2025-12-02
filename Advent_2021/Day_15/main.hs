module Main where

import Data.Char (digitToInt)
import Data.Foldable (toList)
import Data.Maybe (fromMaybe)
import Data.Sequence qualified as Seq
import Data.Set qualified as Set
import Prelude hiding (Left, Right)

data Direction = Up | Down | Left | Right deriving (Eq, Show)

mapDirToTuple dir
  | dir == Up = (-1, 0)
  | dir == Down = (1, 0)
  | dir == Left = (0, -1)
  | dir == Right = (0, 1)
  | otherwise = (0, 0)

linesToGrid lines = [[digitToInt a | a <- line] | line <- lines]

isInBounds :: (Int, Int) -> [[Int]] -> Bool
isInBounds node grid = fst node >= 0 && fst node < length grid && snd node >= 0 && snd node < length (head grid)

getNeighbours node grid = filter (`isInBounds` grid) [addTuples node dir | dir <- [(1, 0), (-1, 0), (0, -1), (0, 1)]]

addTuples t1 t2 = (fst t1 + fst t2, snd t1 + snd t2)

bfs grid start goal = bfsHelper (Seq.singleton start) Set.empty
  where
    rows = length grid
    cols = length (head grid)

    bfsHelper queue visited
      | Seq.null queue = []
      | otherwise =
          let (current Seq.:< restQueue) = Seq.viewl queue
              newVisited = Set.insert current visited
              validNeighbors = [p | p <- getNeighbours current grid, p `Set.notMember` visited]
              newQueue = restQueue Seq.>< Seq.fromList validNeighbors
           in current : bfsHelper newQueue newVisited

main = do
  file <- readFile "test.txt"
  let file_lines = lines file
  let startPos = (0, 0)
  let grid = linesToGrid file_lines
  let endPos = (length grid - 1, length (head grid) - 1)

  print (getNeighbours startPos grid)

  print (bfs grid startPos endPos)