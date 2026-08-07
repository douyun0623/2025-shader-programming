# OpenGL Shader Programming Lab

> C++/OpenGL 렌더러에 파티클, 다중 렌더 타깃과 후처리 파이프라인을 단계적으로 구현한 개인 그래픽스 학습 프로젝트입니다.

<p align="center">
  <img src="docs/images/bloom.png" width="512" alt="Bloom 파이프라인 실행 결과">
</p>

<p align="center">
  최종 bloom 합성 화면과 디버그 렌더 타깃(왼쪽 아래: 밝은 영역, 오른쪽 아래: 블러 결과)
</p>

## 프로젝트 개요

| 항목 | 내용 |
|---|---|
| 개발 기간 | 2025.09 ~ 2025.12 |
| 형태 | 개인 학습 프로젝트 |
| 플랫폼 | Windows |
| 기술 | C++, OpenGL, GLSL 3.30, GLEW, freeglut |
| 개발 환경 | Visual Studio 2022, MSVC v143 |
| 현재 결과 | HDR 파티클 → bright-pass → ping-pong blur → tone mapping/bloom 합성 |

수업에서 제공된 최소 OpenGL 프레임워크를 출발점으로 사용했습니다. 이후 29개 커밋을 통해 파티클 데이터, 메시와 텍스처, GLSL 효과, FBO/MRT, bloom 렌더 패스를 직접 확장했습니다.

## 대표 구현

### GPU 파티클

- 2,500개 파티클을 6개 정점의 쿼드로 구성해 하나의 VBO에 저장
- 위치, 색상, 시작 시각, 속도, 수명, 질량, 주기와 UV를 정점 속성으로 전달
- vertex shader에서 원형·사인 곡선·중력 기반 이동과 수명별 알파 계산
- fragment shader에서 파티클 텍스처와 색을 결합하고 밝기 임계값을 판정

### MRT와 bloom

현재 실행 경로는 다음 순서로 렌더링합니다.

1. `GL_RGBA16F` HDR FBO의 두 color attachment에 원본 파티클과 밝은 영역을 동시에 기록
2. 밝기 값이 1을 넘는 픽셀만 bloom 입력 텍스처로 분리
3. 두 ping-pong FBO를 오가며 가로·세로 Gaussian blur 반복
4. 원본 HDR 텍스처와 블러 텍스처를 합성
5. exponential tone mapping과 gamma correction을 적용해 화면에 출력

하단의 두 디버그 뷰를 통해 bright-pass와 블러 중간 결과를 최종 합성과 한 화면에서 비교할 수 있습니다.

### 셰이더 실습 확장

- 500×500 grid mesh와 파동·깃발 형태 vertex 변형
- 여러 texture unit을 활용한 숫자·이미지 합성
- FBO와 다중 color attachment를 이용한 off-screen rendering
- 렌즈, 어안, 중력 렌즈, 빗방울, 색수차, CRT, 필름, 픽셀화 등 fragment shader 실험
- 실행 중 `1` 키로 shader program 재컴파일

이 효과들은 [`Shaders`](SimpleGame/Shaders) 폴더에 남아 있으며, 현재 기본 실행은 bloom 파이프라인에 맞춰져 있습니다.

## 코드 흐름

| 단계 | 구현 위치 | 역할 |
|---|---|---|
| 창·렌더 루프 | [`SimpleGame.cpp`](SimpleGame/SimpleGame.cpp) | freeglut 초기화, 프레임 호출, shader reload 입력 |
| 렌더 리소스 | [`Renderer.cpp`](SimpleGame/Renderer.cpp) | VBO, texture, FBO/MRT와 draw pass 관리 |
| 파티클 이동 | [`particle.vs`](SimpleGame/Shaders/particle.vs) | 시간과 파티클 속성으로 정점 위치 계산 |
| 밝은 영역 분리 | [`particle.fs`](SimpleGame/Shaders/particle.fs) | 두 render target에 원본과 bright-pass 출력 |
| 블러·합성 | [`Texture.fs`](SimpleGame/Shaders/Texture.fs) | Gaussian blur, tone mapping, gamma correction |

## 실행 방법

### 요구 사항

- Visual Studio 2022의 **Desktop development with C++** 워크로드
- OpenGL 3.3 이상을 지원하는 그래픽 드라이버
- 저장소에 포함된 x64용 `freeglut.dll`, `glew32.dll`

### Visual Studio

1. [`SimpleGame.sln`](SimpleGame.sln)을 엽니다.
2. 구성을 `Release`, 플랫폼을 `x64`로 선택합니다.
3. shader와 texture의 상대 경로를 찾도록 **Debugging > Working Directory**를 `$(ProjectDir)`로 설정합니다.
4. 빌드 후 실행합니다.

### Developer PowerShell

```powershell
msbuild .\SimpleGame.sln /m /p:Configuration=Release /p:Platform=x64
Set-Location .\SimpleGame
..\x64\Release\SimpleGame.exe
```

| 입력 | 동작 |
|---|---|
| `1` | 모든 GLSL program 다시 읽기·컴파일 |
| 창 닫기 | 프로그램 종료 |

## 검증 결과

2026.08.08 기준 Visual Studio Community 2022와 MSVC v143으로 확인했습니다.

| 검증 항목 | 결과 |
|---|---|
| `Release | x64` 빌드 | 성공, 오류 0건 |
| 실행 및 OpenGL 창 생성 | 성공 |
| 5초 이상 프로세스 응답 | 확인 |
| bloom 최종 합성 | 실행 화면으로 확인 |
| bright-pass·blur 디버그 뷰 | 실행 화면으로 확인 |

현재 빌드에는 15개의 경고가 있습니다. 대부분 포함된 LodePNG 코드의 정수 변환 경고이며, `Renderer.cpp`에도 사용되지 않는 식과 정수/실수 변환 경고가 남아 있습니다.

## 제공 코드와 직접 구현 범위

초기 프레임워크의 [`SimpleGame.cpp`](SimpleGame/SimpleGame.cpp)에는 이태희 교수의 저작권 및 사용 조건이 기재되어 있습니다. 초기 base commit과 현재 코드를 비교하면 이후 다음 항목이 추가되었습니다.

- `Renderer.cpp` 약 980줄 확장
- 파티클·grid·full-screen·texture shader 세트 추가
- PNG texture 로딩, 다중 texture, FBO/MRT, 후처리와 bloom 파이프라인 추가

PNG 디코더는 Lode Vandevenne의 **LodePNG 20170917**을 사용하며 고지문은 [`LoadPng.cpp`](SimpleGame/LoadPng.cpp)에 보존되어 있습니다. GLEW와 freeglut도 외부 의존성입니다.

## 알려진 한계

- FBO와 viewport 크기가 512×512로 고정되어 창 크기 변경을 지원하지 않습니다.
- 애니메이션 시간이 실제 delta time이 아니라 프레임마다 고정값으로 증가해 프레임 속도에 영향을 받습니다.
- 여러 실습 효과를 런타임 메뉴가 아니라 draw call과 shader 주석으로 선택합니다.
- shader program을 다시 불러올 때 program과 GPU 리소스를 완전히 해제하지 않아 장시간 반복 시 누수 가능성이 있습니다.
- 외부 라이브러리 바이너리가 저장소에 포함되어 있어 배포 전 각 라이선스와 최신 버전을 다시 확인해야 합니다.

## 프로젝트 구조

```text
.
├─ SimpleGame.sln
├─ SimpleGame/
│  ├─ SimpleGame.cpp       # 창·입력·렌더 루프
│  ├─ Renderer.cpp/.h      # 렌더 리소스와 draw pass
│  ├─ LoadPng.cpp/.h       # LodePNG 기반 texture 로딩
│  ├─ Shaders/             # GLSL vertex/fragment shaders
│  └─ Dependencies/        # GLEW/freeglut headers·libraries
├─ docs/images/bloom.png   # 실제 실행 캡처
└─ README.md
```
