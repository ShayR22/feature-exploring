#!/bin/python3

import requests
import os
import re

CATS_HTTP_BASE_URL = "https://http.cat"
BASE_CATS_HTTP_IMAGE_DIR = "cats_status_codes"

STATUS_CODES_TO_DESC={100:"continue",101:"switching_protocols",102:"processing",103:"checkpoint",122:"uri_too_long",200:"ok",201:"created",202:"accepted",203:"non_authoritative_info",204:"no_content",205:"reset_content",206:"partial_content",207:"multi_status",208:"already_reported",226:"im_used",300:"multiple_choices",301:"moved_permanently",302:"found",303:"see_other",304:"not_modified",305:"use_proxy",306:"switch_proxy",307:"temporary_redirect",308:"permanent_redirect",400:"bad_request",401:"unauthorized",402:"payment_required",403:"forbidden",404:"not_found",405:"method_not_allowed",406:"not_acceptable",407:"proxy_authentication_required",408:"request_timeout",409:"conflict",410:"gone",411:"length_required",412:"precondition_failed",413:"request_entity_too_large",414:"request_uri_too_large",415:"unsupported_media_type",416:"requested_range_not_satisfiable",417:"expectation_failed",418:"im_a_teapot",420:"enhance_your_calm",421:"misdirected_request",422:"unprocessable_entity",423:"locked",424:"failed_dependency",425:"unordered_collection",426:"upgrade_required",428:"precondition_required",429:"too_many_requests",431:"header_fields_too_large",444:"no_response",449:"retry_with",450:"blocked_by_windows_parental_controls",451:"unavailable_for_legal_reasons",497:"http_request_sent_to_https_port",498:"token_expired",499:"client_closed_request",500:"internal_server_error",501:"not_implemented",502:"bad_gateway",503:"service_unavailable",504:"gateway_timeout",505:"http_version_not_supported",506:"variant_also_negotiates",507:"insufficient_storage",508:"loop_detected",509:"bandwidth_limit_exceeded",510:"not_extended",511:"network_authentication_required",521:"web_server_is_down",522:"connection_timed_out",523:"origin_is_unreachable",525:"ssl_handshake_failed",599:"network_connection_timeout_error"}

# the reason this is jpg is because this is what the site had
def download_jpg_cat_image(status_code: int, base_name:str=CATS_HTTP_BASE_URL) -> bytes:
    image_url_str = base_name + "/" + str(status_code)
    return requests.get(image_url_str).content


def gen_jpg_image_path(status_code: int, desc: str, base_dir: str=BASE_CATS_HTTP_IMAGE_DIR) -> str:
    """
    generate a full path name to save image path at based on status code, status code description
    and a base path, for example status_code 404 would generate:
    cats_status_codes/400/404_not_found.jpg
    """
    status_code_dir_name = str(int(status_code / 100) * 100)
    image_name = str(status_code) + "_" + desc + ".jpg"
    return base_dir + "/" + status_code_dir_name + "/" + image_name


def save_image(full_path: str, image_data: bytes):
    # create path, if path doesn't exists
    dirname = os.path.dirname(full_path)
    if not os.path.exists(dirname):
        os.makedirs(dirname)

    with open(full_path, 'wb') as fp:
        fp.write(image_data)


def download_all_status_codes_and_save_them(status_codes: list):
    print("Starting to iterate over each cat http status code image downlaoding and saving it")
    for status_code in status_codes:
        cat_jpg_image_data = download_jpg_cat_image(status_code)

        # check if status_code found in cats page is a real status code
        if status_code in STATUS_CODES_TO_DESC:
            status_code_description = STATUS_CODES_TO_DESC[status_code]
        else:
            print(f"{status_code=} not found, using 'custom' as description for image name")
            status_code_description = "custom"

        path_to_save = gen_jpg_image_path(status_code, status_code_description)

        save_image(path_to_save, cat_jpg_image_data)

        print(f"saved: {path_to_save}")


def get_all_available_status_codes(base_url: str=CATS_HTTP_BASE_URL) -> list:
    """
    get page all cats image status codes by looking for the href for the specific link
    and extracting the numbers
    """
    req = requests.get(base_url)
    text = req.text

    pattern = r'href=\"\/\d{3}\"'
    href_numbers = re.findall(pattern, text)
    # each href_number has the number in -4:-1
    numbers = [int(href[-4:-1]) for href in href_numbers]

    return numbers


if __name__ == '__main__':
    cat_status_codes_images = get_all_available_status_codes()
    print(f"{cat_status_codes_images=}")
    download_all_status_codes_and_save_them(cat_status_codes_images)

