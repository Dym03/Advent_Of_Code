import requests
from bs4 import BeautifulSoup
import sys

year = sys.argv[1]
day = sys.argv[2]
URL = f"https://adventofcode.com/{year}/day/{day}/input"

cookie = {'session':'53616c7465645f5f18fa22075973209a223c2afb77b0398ca1b976ede689e4e5e8011f649dc6c0a20bdf8fa6c581376cc852051ab54acddac8b4a5aee725f8d3'}

page = requests.post(URL, cookies=cookie)

input = BeautifulSoup(page.content, 'html.parser')

with open('input.txt', 'w+') as file:
    file.write(input.text.rstrip())

