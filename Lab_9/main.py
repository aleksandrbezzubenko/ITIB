import io
import json
import string
from tabulate import tabulate
from typing import List


class TableCreator:
    def __init__(self, headers: List[str], rows: List[List[str]] = None):
        self.rows = [headers]

        if rows is not None:
            for row in rows:
                self.rows.append(row)

        self.column_quantity = len(headers)
        self.headers = headers

    def add_row(self, row: List[str]):
        if len(row) != self.column_quantity:
            raise ValueError

        self.rows.append(row)

    def print_table(self, output, header: str = None, table_fmt: str = None, show_index: bool = None):
        output.write(tabulate(self.rows, headers=header, tablefmt=table_fmt, showindex=show_index))


district_centers = {
    'Новомосковский административный округ': [37.2100, 55.3400],
    'Троицкий административный округ': [37.1500, 55.2800],
    'Восточный административный округ': [37.786369, 55.796039],
    'Западный административный округ': [37.468372, 55.711506],
    'Зеленоградский административный округ': [37.194273, 55.989725],
    'Северный административный округ': [37.529975, 55.834950],
    'Северо-Восточный административный округ': [37.628449, 55.851847],
    'Северо-Западный административный округ': [37.433551, 55.821479],
    'Центральный административный округ': [37.623374, 55.753576],
    'Юго-Восточный административный округ': [37.768235, 55.689028],
    'Юго-Западный административный округ': [37.537672, 55.639672],
    'Южный административный округ': [37.661130, 55.641486]
}


def euclidean_distance(x1, y1, x2, y2) -> float:
    return ((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5


class District:
    def __init__(self, district_name: str, center_coordinate_x: float,
                 center_coordinate_y: float):
        self._district_name = district_name
        self._coordinate_x = center_coordinate_x
        self._coordinate_y = center_coordinate_y

    @property
    def district_name(self):
        return self._district_name

    def print_district(self):
        print(f"District name: {self._district_name}")
        print(f"Center coordinates: {self._coordinate_x:.4f}, {self._coordinate_y:.4f}")

    @property
    def coordinate_x(self):
        return self._coordinate_x

    @property
    def coordinate_y(self):
        return self._coordinate_y


class Please:
    def __init__(self, short_name: string, coordinate_x: float,
                 coordinate_y: float, district_name: str):
        self._short_name = short_name
        self._coordinate_x = coordinate_x
        self._coordinate_y = coordinate_y
        self._district_name = district_name

    @property
    def short_name(self):
        return self._short_name

    @property
    def district_name(self):
        return self._district_name

    def distance_between_district_center(self, center: District, distance_func) -> float:
        return distance_func(self._coordinate_x, self._coordinate_y, center.coordinate_x, center.coordinate_y)

    def is_belong_to_district(self, center: str) -> bool:
        return self._district_name == center


def init_district_centers(centers) -> List[District]:
    result = []
    for district_name, coordinates in centers.items():
        if len(coordinates) != 2:
            raise ValueError
        result.append(District(district_name, coordinates[0], coordinates[1]))
    return result


def read_json(file_name: string, mode: string):
    with open(file_name + '.json', mode) as file:
        data = json.load(file)
    return data


def init_pleases(data) -> List[Please]:
    pleases = []

    for block in data['vel']:
        coordinates = block['geoData']['coordinates']
        if len(coordinates) != 2:
            raise ValueError
        pleases.append(Please(block["Name"], coordinates[0], coordinates[1], block["AdmArea"]))

    return pleases


class Statistics:
    def __init__(self, quantity: int):
        self._quantity = quantity
        self._failure_counter = 0
        self._failure_cases = dict()
        self._max_please_name_width = 0
        self._max_district_name_width = 0
        self._table = TableCreator(['Название учреждения', 'Округ по алгоритму', 'Округ по факту', 'Совпадение'])

    def add_table_row(self, row: List[str]):
        self._table.add_row(row)

    def get_statistic(self, output):
        self._table.print_table(output, header='firstrow', table_fmt='fancy_grid', show_index=True)

        output.write(f'\n\nВсего: {self._quantity}\n')
        output.write(f'Верно кластеризованных: {self._quantity - self._failure_counter}\n')
        output.write(f'Неверно кластеризованных: {self._failure_counter}\n')
        output.write(f'Процент ошибок: {(self._failure_counter / self._quantity * 100):.2f} %\n')
        i = 1
        for name, tup in self._failure_cases.items():
            output.write(f'{i}. Учреждение: {name}. По алгоритму: {tup[0]}. По факту: {tup[1]}\n')
            i += 1

    def increment_failure(self):
        self._failure_counter += 1

    def add_failure_case(self, please_name: str, expected_district: str, actual_district: str):
        self._failure_cases[please_name] = (expected_district, actual_district)


def analysis(pleases: List[Please], centers: List[District]) -> Statistics:
    if len(pleases) == 0 or len(centers) == 0:
        raise ValueError

    stats = Statistics(len(pleases))
    for element in pleases:
        row = [element.short_name, element.district_name]

        min_distance = element.distance_between_district_center(centers[0], euclidean_distance)
        min_district = centers[0].district_name

        for district_center in centers:
            current_distance = element.distance_between_district_center(district_center, euclidean_distance)
            if current_distance < min_distance:
                min_distance = current_distance
                min_district = district_center.district_name

        row.append(min_district)
        if not element.is_belong_to_district(min_district):
            stats.increment_failure()
            stats.add_failure_case(element.short_name, min_district, element.district_name)

            row.append('✗')
        else:
            row.append('✓')
        stats.add_table_row(row)
    return stats


if __name__ == '__main__':
    districts = init_district_centers(district_centers)

    j_file = read_json('data', 'r')
    pleases = init_pleases(j_file)

    with io.open('results.txt', 'w', encoding="utf-8") as file:
        analysis(pleases, districts).get_statistic(file)
