import Data.List (transpose)

getBoards [] boards = boards
getBoards lines boards = do
  let board = take 5 $ drop 1 lines
  let board_2 = map words board
  let rest = drop 6 lines
  getBoards rest (boards ++ [board_2])

markLine :: [String] -> String -> [String]
markLine line c = do
  let line' = map (\x -> if x == c then "X" else x) line
  line'

markBoard :: [[String]] -> String -> [[String]]
markBoard board c = do
  let board' = map (\x -> markLine x c) board
  board'

markBoards :: [[[String]]] -> String -> [[[String]]]
markBoards boards c = do
  let boards' = map (\x -> markBoard x c) boards
  boards'

findSolution :: [String] -> [[[String]]] -> [String] -> [[[String]]] -> Int
findSolution used_nums boards [] solved = 0
findSolution used_nums boards nums [] = do
  let num = head nums
  let boards' = markBoards boards num
  let solved = filter isSolved boards'
  let used_nums' = used_nums ++ [num]
  findSolution used_nums' boards' (tail nums) solved
findSolution used_nums boards nums solved = (read $ last used_nums :: Int) * getSumOfBoard (head solved)

findSolution_p2 used_nums boards [] solved = 0
findSolution_p2 used_nums [] _ solved = (read $ last used_nums :: Int) * getSumOfBoard (last solved)
findSolution_p2 used_nums boards nums solved = do
  let num = head nums
  let boards' = markBoards boards num
  let solved' = solved ++ filter isSolved boards'
  let boards'' = filter (\x -> notElem x solved') boards'
  let used_nums' = used_nums ++ [num]
  findSolution_p2 used_nums' boards'' (tail nums) solved'

-- findSolution_p2 used_nums boards [] solved = 0
-- findSolution_p2 used_nums [] _ solved = read $ last used_nums
-- findSolution_p2 used_nums boards nums solved = do
--   let num = head nums
--   let boards' = markBoards boards num
--   let newly_solved = filter isSolved boards'
--   let solved' = solved ++ newly_solved
--   let boards'' = filter (\x -> notElem x solved') boards'
--   let used_nums' = used_nums ++ [num]
--   findSolution_p2 used_nums' boards'' (tail nums) solved'

getSumOfBoard :: [[String]] -> Int
getSumOfBoard [] = 0
getSumOfBoard board = sum $ map read $ filter (/= "X") $ concat board

-- isSolved :: [[String]] -> Bool

isSolved [] = False
isSolved board = do
  let board' = map (\x -> filter (\y -> y == "X") x) board
  let board'' = filter (\x -> length x == 5) board'
  let t_board = transpose board
  let t_board' = map (\x -> filter (\y -> y == "X") x) t_board
  let t_board'' = filter (\x -> length x == 5) t_board'
  length board'' > 0 || length t_board'' > 0

splitByInterpunction :: String -> [String]
splitByInterpunction [] = []
splitByInterpunction s = do
  let (word, rest) = break (\x -> x == ',' || x == '.' || x == '!' || x == '?') s
  if null rest then [word] else word : splitByInterpunction (tail rest)

main = do
  file <- readFile "input.txt"
  let first_line = splitByInterpunction $ head $ lines file
  let rest = tail $ lines file
  let boards = getBoards rest []
  -- print first_line
  -- print boards

  -- print $ map (map (\x -> if x == "22" then "X" else x)) (head boards)
  -- print (markBoards boards ([head first_line]))
  print $ findSolution [] boards first_line []
  print $ findSolution_p2 [] boards first_line []