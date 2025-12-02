module Main where

import Data.Bits (Bits (shiftL))

convertToBinary x
  | x == '0' = "0000"
  | x == '1' = "0001"
  | x == '2' = "0010"
  | x == '3' = "0011"
  | x == '4' = "0100"
  | x == '5' = "0101"
  | x == '6' = "0110"
  | x == '7' = "0111"
  | x == '8' = "1000"
  | x == '9' = "1001"
  | x == 'A' = "1010"
  | x == 'B' = "1011"
  | x == 'C' = "1100"
  | x == 'D' = "1101"
  | x == 'E' = "1110"
  | x == 'F' = "1111"

convertFromBinary [] n = n
convertFromBinary [x] n = if x == '1' then n + 1 else n
convertFromBinary (x : xs) n = if x == '1' then convertFromBinary xs ((n + 1) * 2) else convertFromBinary xs (n * 2)

parseVersionId binary = (version, id, updatedBinary)
  where
    version = convertFromBinary (take 3 binary) 0
    id = convertFromBinary (take 3 (drop 3 binary)) 0
    updatedBinary = drop 6 binary

splitIntoFours [] = []
splitIntoFours list
  | length list > 5 = drop 1 (take 5 list) ++ splitIntoFours (drop 5 list)
  | otherwise = []

-- lengthTypeId = convertFromBinary (take 1 (drop 6 binary)) 0
-- totalPacketLength = convertFromBinary (take 15 (drop 7 binary)) 0
-- numberOfSubPackets = convertFromBinary (take 11 (drop 7 binary)) 0
-- version

parsePacket binary =
  if packetId == 4
    then convertFromBinary (splitIntoFours updatedBinary) 0
    else
      let lengthTypeId = convertFromBinary (take 1 updatedBinary) 0
       in if lengthTypeId == 0
            then
              let totalLengthOfPackets = convertFromBinary (take 15 (drop 1 updatedBinary)) 0
               in totalLengthOfPackets
            else
              let numberOfPackets = convertFromBinary (take 11 (drop 1 updatedBinary)) 0
               in numberOfPackets
  where
    version = convertFromBinary (take 3 binary) 0
    packetId = convertFromBinary (take 3 (drop 3 binary)) 0
    updatedBinary = drop 6 binary

main = do 
  file <- readFile "test.txt"
  let fileLines = lines file
  print fileLines
  let binaryFile = concatMap convertToBinary (head fileLines)
  print binaryFile