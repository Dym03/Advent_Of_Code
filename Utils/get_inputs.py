import requests
import sys

year = sys.argv[1]
day = sys.argv[2]
URL = f"https://adventofcode.com/{year}/day/{day}/input"

session_id = "53616c7465645f5f0af2bfea3a3d9d4af9b22042f5a701fe993543ca7bb7d4e90223ad2baed8154d5e3b6551db6caff3a1844f0a2ef32672cf73aa878944cc37"

cookie = {"session": session_id}

page = requests.get(URL, cookies=cookie)

input = page.text

with open("input.txt", "w+") as file:
    file.write(input.rstrip())
