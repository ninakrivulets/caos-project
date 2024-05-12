import httpx
from httpx import Client
import json
from pprint import pprint
#
#
# url = 'http://recepty-po-ingredientam.ru/api/'
#
# params = {
#     'service': 'Parser',
#     'method': 'SearchIngredients',
#     'data': {
#         'search': 'яйц',
#         'token': None,
#         'time': '7a4fd9e0b329284a82b1c4dbc52076c4cf3ade56fb6e5571d37e920d1d1e302b',
#         'signature': 'd30a86367f5f57852327ea3c9ecc7d3702b72298136c79c259be31d3203c4cde',
#     },
# }
#
#
# session = Client()
# # session.headers = headers
# response = session.post(url, json=params)
# data = response.json()
# to_choose = []
# tmp_ids = {}
# for x in data['data']['items']:
#     pprint(x)
#     tmp_ids[x['name']] = x['id']
#     to_choose.append(x['name'])
#
# pprint(to_choose)



import requests
#
# cookies = {
#     '_ym_uid': '17149927058195730',
#     '_ym_d': '1714992705',
#     '_ym_isad': '2',
#     '_gid': 'GA1.2.1155331457.1714992705',
#     '_ym_visorc': 'w',
#     '_ga_VWRTSDQPZ2': 'GS1.1.1714992704.1.1.1714993813.0.0.0',
#     '_ga': 'GA1.1.463134648.1714992705',
# }

# headers = {
#     'Accept': '*/*',
#     'Accept-Language': 'ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7',
#     'Connection': 'keep-alive',
#     'Content-type': 'application/json; charset=UTF-8',
#     # 'Cookie': '_ym_uid=17149927058195730; _ym_d=1714992705; _ym_isad=2; _gid=GA1.2.1155331457.1714992705; _ym_visorc=w; _ga_VWRTSDQPZ2=GS1.1.1714992704.1.1.1714993813.0.0.0; _ga=GA1.1.463134648.1714992705',
#     'Origin': 'http://recepty-po-ingredientam.ru',
#     'Referer': 'http://recepty-po-ingredientam.ru/',
#     'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/123.0.0.0 Safari/537.36',
# }

json_data = {
    'service': 'Parser',
    'method': 'Search',
    'data': {
        'ingredients': [
            '28',
            '73',
        ]
    }
}

response = requests.post('http://recepty-po-ingredientam.ru/api/', json=json_data, verify=False)
pprint(response.json())