# Unreal Engine 5.5 기반 ARPG 게임 제작

본 프로젝트는 **Unreal Engine 5.5**를 기반으로 제작하고 있는 **검을 활용한 액션 RPG (ARPG)** 게임입니다.

<br>

## 주요 시스템

### 1. 상태 기반 입력 시스템

플레이어의 상태에 따라 **Input Mapping Context**를 동적으로 변경하여, **상태별 고유한 공격 패턴**을 구성합니다.

- 예시:
  - `Idle` 상태: 기본 공격 콤보
  - `Walk` 상태: 이동 공격 콤보
  - `Run` 상태: 에어본 공격 콤보
  - 'Focusing' 상태: 돌진 공격

이를 통해 **상황에 따른 전략적인 전투 방식**을 유도하고, 플레이어의 행동 다양성을 높였습니다.

<br>

### 2. 피격 효과 다양화

플레이어의 공격 패턴에 따라 **적의 피격 반응 또한 변화**합니다.

- 예시:
  - 가벼운 일격 → 피격 애니메이션
  - 에어본 공격 → 에어본 및 추락

<br>

## 기능 요약

- **Unreal Engine 5.5**
- **Enhanced Input System**을 활용한 입력 처리
- **AnimMontage**를 통한 콤보 구성

<br>

## 🔗 시연 영상

프로젝트 시연 영상은 아래 링크를 통해 확인하실 수 있습니다:  
[시연 영상 보기 (Google Drive)](https://drive.google.com/file/d/1WdGS_iOjOS8wM3dcLjuFmD8O_BXH-Vsi/view?usp=sharing)

