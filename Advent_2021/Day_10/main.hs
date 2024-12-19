module Main where
import Data.List (sort)

solveLineP1 :: String -> Int
solveLineP1 line = go line []
  where
    go :: String -> [Char] -> Int
    go [] [] = 0
    go [] q = 0
    go (x : xs) q
      | x `elem` ['{', '(', '[', '<'] = go xs (x : q)
      | x == ')' && head q == '(' = go xs (tail q)
      | x == ')' && head q /= '(' = 3
      | x == '}' && head q == '{' = go xs (tail q)
      | x == '}' && head q /= '{' = 1197
      | x == ']' && head q == '[' = go xs (tail q)
      | x == ']' && head q /= '[' = 57
      | x == '>' && head q == '<' = go xs (tail q)
      | x == '>' && head q /= '<' = 25137
      | otherwise = go xs q

solveP1 = sum . map solveLineP1


solveLineP2 line = evaluate (go line []) 0
  where
    go :: String -> [Char] -> [Char]
    go [] [] = []
    go [] q = q
    go (x : xs) q
      | x `elem` ['}', ')', ']', '>'] && head q /= x = []
      | x == '(' = go xs (')' : q)
      | x == '{' = go xs ('}' : q)
      | x == '[' = go xs (']' : q)
      | x == '<' = go xs ('>' : q)
      | otherwise = go xs (tail q)
    evaluate :: [Char] -> Int -> Int
    evaluate [] n = n
    evaluate (x : xs) n
      | x == ')' = evaluate xs (5 * n + 1)
      | x == ']' = evaluate xs (5 * n + 2)
      | x == '}' = evaluate xs (5 * n + 3)
      | x == '>' = evaluate xs (5 * n + 4)
      | otherwise = evaluate xs n

solveP2 input = solution !! (length solution `div` 2)
  where
    solution = sort $ filter (/= 0) (map solveLineP2 input)

main = do
  file <- readFile "test.txt"
  let input = lines file
  print $ solveP1 input
  print $ solveP2 input
