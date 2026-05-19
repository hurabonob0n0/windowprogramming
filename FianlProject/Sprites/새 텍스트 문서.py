from PIL import Image
import numpy as np

def fix_alpha(input_path, output_path):
    # 이미지를 RGBA 모드로 열기
    img = Image.open(input_path).convert('RGBA')
    data = np.array(img)

    # 알파 채널(4번째 값) 추출
    alpha = data[:, :, 3]

    # 완전 투명(0)이 아닌 픽셀은 모두 255(완전 불투명)로 덮어쓰기
    alpha[alpha > 0] = 255

    # 변경된 알파 채널을 다시 적용
    data[:, :, 3] = alpha

    # 결과 저장 (투명도를 유지하려면 반드시 PNG로 저장)
    result_img = Image.fromarray(data)
    result_img.save(output_path, "PNG")
    print("알파값 변환 완료!")

# 사용 예시 (입력 파일이 PNG일 때)
fix_alpha("Explode_64_8_64.png", "Explode_64_8_64_fixed.png")