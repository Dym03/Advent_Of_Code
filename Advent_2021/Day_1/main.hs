module Main where

getIncreasing :: (Ord a, Num t) => [a] -> t -> t
getIncreasing [] c = c
getIncreasing [x] c = c
getIncreasing (x : xs) c = if x <= head xs then getIncreasing xs (c + 1) else getIncreasing xs c

getIncreasingSums :: (Ord a, Num a, Num t) => [a] -> [a] -> t -> t
getIncreasingSums _ [] c = c
getIncreasingSums prev_3 [x] c = if sum prev_3 < sum (tail prev_3 ++ [x]) then c + 1 else c
getIncreasingSums prev_3 (x : xs) c
  | length prev_3 < 3 = getIncreasingSums (prev_3 ++ [x]) xs c
  | length prev_3 == 3 && sum prev_3 < sum (tail prev_3 ++ [x]) = getIncreasingSums (tail prev_3 ++ [x]) xs (c + 1)
  | otherwise = getIncreasingSums (tail prev_3 ++ [x]) xs c

main :: IO ()
main = do
  s <- readFile "input.txt"
  let l = lines s
  print $ getIncreasing (map read (l :: [String]) :: [Int]) 0
  print $ getIncreasingSums [] (map read (l :: [String]) :: [Int]) 0
