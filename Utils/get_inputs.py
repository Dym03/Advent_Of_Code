import requests
import sys

year = sys.argv[1]
day = sys.argv[2]
URL = f"https://adventofcode.com/{year}/day/{day}/input"

session_id = "53616c7465645f5f8adcdcadf72769dbe2e247ade9baa5399f0a4f39a2dd722dd1cab365d85342df8bfae4231cdd5beee09632a7e3c3880dd35fddad59558979"

cookie = {"session": session_id}

page = requests.get(URL, cookies=cookie)

input = page.text

with open("input.txt", "w+") as file:
    file.write(input.rstrip())
