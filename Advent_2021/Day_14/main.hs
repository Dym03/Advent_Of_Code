module Main where

import Control.Monad.State
import Data.Map (Map, fromList, fromListWith, insert, member, toList, (!))
import Data.Map qualified as Map
import Data.Text (pack, splitOn, unpack)

parseRule line = (head result, last result)
  where
    result = map unpack $ splitOn (pack " -> ") (pack line)

parseInput inp = (base, rules)
  where
    base = head inp
    rules = fromList $ map parseRule (drop 1 $ tail inp)

insertMiddle str middle last
  | last = take 1 str ++ middle ++ drop 1 str
  | otherwise = take 1 str ++ middle

cache :: Map String String
cache = fromList []

generatePairs :: String -> Map String String -> Map String String -> (String, Map String String)
generatePairs [] _ cache = ("", cache)
generatePairs [x] _ cache = ([x], cache)
generatePairs str rules cache
  | member str cache = (cache ! str, cache)
  | length str > 2 =
      let newPair = take 2 str
          rest = drop 1 str
          applied = applyRule newPair rules False
          result = applied ++ fst (generatePairs rest rules updatedCache)
          updatedCache = insert str result cache
       in (result, updatedCache)
  | otherwise =
      let newPair = take 2 str
          result = applyRule newPair rules True
          updatedCache = insert str result cache
       in (result, updatedCache)
  where
    applyRule pair rules last
      | member pair rules = insertMiddle pair (rules ! pair) last
      | otherwise = pair

applySteps 0 str rules c = str
applySteps n str rules c = applySteps (n - 1) (fst result) rules (snd result)
  where
    result = generatePairs str rules c

calculateScore str = most - least
  where
    counter = toList $ (fromListWith (+) . map (,1)) str
    least = minimum (map snd counter)
    most = maximum (map snd counter)

main = do
  input <- readFile "test.txt"
  let l_input = lines input
  let parsedInput = parseInput l_input
  let polymer = uncurry (applySteps 15) parsedInput cache

  print $ calculateScore polymer