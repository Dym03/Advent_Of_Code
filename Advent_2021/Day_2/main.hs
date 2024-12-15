import Data.Text (split)

intoWords = map words

intoTuple x = (head x, read (last x) :: Int)

play :: Int -> Int -> [(String, Int)] -> Int
play x_pos y_pos [] = x_pos * y_pos
play x_pos y_pos ((x, y) : xs)
  | x == "forward" = play (x_pos + y) y_pos xs
  | x == "up" = play x_pos (y_pos - y) xs
  | x == "down" = play x_pos (y_pos + y) xs
  | otherwise = x_pos * y_pos

play_p_2 :: Int -> Int -> Int -> [(String, Int)] -> Int
play_p_2 x_pos y_pos aim [] = x_pos * y_pos
play_p_2 x_pos y_pos aim ((x, y) : xs)
  | x == "forward" = play_p_2 (x_pos + y) (y_pos + aim * y) aim xs
  | x == "up" = play_p_2 x_pos y_pos (aim - y) xs
  | x == "down" = play_p_2 x_pos y_pos (aim + y) xs
  | otherwise = x_pos * y_pos

main = do
  input <- readFile "test.txt"
  let l = lines input
  let l1 = map intoTuple (intoWords l)
  print (play 0 0 l1)
  print (play_p_2 0 0 0 l1)
