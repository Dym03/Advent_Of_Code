import Data.Array (Ix (index))
import Data.List (elemIndex, transpose)
import Data.Maybe (fromJust)

mostCommon str = if length (filter (== '1') str) >= length (filter (== '0') str) then '1' else '0'

leastCommon str = if length (filter (== '1') str) < length (filter (== '0') str) then '1' else '0'

fromBinary :: String -> Int
fromBinary = foldl (\acc x -> 2 * acc + digitToInt x) 0
  where
    digitToInt c = if c == '0' then 0 else 1

getResult gamma epsilon = fromBinary gamma * fromBinary epsilon

filterOxygene :: [String] -> Int -> String
filterOxygene [l] _ = l
filterOxygene l ind =
  let gamma = mapOnColumns l mostCommon
   in filterOxygene (filterFirstChar gamma l ind) (ind + 1)

filterCO2 [l] _ = l
filterCO2 l ind =
  let gamma = mapOnColumns l leastCommon
   in filterCO2 (filterFirstChar gamma l ind) (ind + 1)

filterFirstChar gamma l ind = filter (\x -> x !! ind == gamma !! ind) l

mapOnColumns :: [[a]] -> ([a] -> b) -> [b]
mapOnColumns lines f = map f (transpose lines)

main = do
  file <- readFile "input.txt"
  let l = lines file
  let gamma = mapOnColumns l mostCommon
  let epsilon = mapOnColumns l leastCommon
  print ("PART 1 " ++ show (getResult gamma epsilon))
  print ("PART 2 " ++ show (getResult (filterOxygene l 0) (filterCO2 l 0)))
