# TP

�^�C�}�[�̐��x���݂邽�߂́u�e�X�g�v���O�����v�ł�.

<picture>
  <img alt="Snapshot" src="Examples/Client/T16P1.png">
</picture>

<p>

�ʌ���[�u�Z���^�C�}�[�l���g���A�v���v](../Bulletin/README.md) ������Ă���Œ���,
�^�C�}�[�̓���̐��m���ɋ^�₪�������̂�,
�^�C�}�[�̖������Ԃ��v�����邽�߂ɍ�����e�X�g�v���O�����ł�.
<sub>( �p���ς񂾂�g���̂Ăɂ������� �gTP�h �ȂǂƂ����S�̂������Ă��Ȃ����O�ō��n�߂��̂ł���,
�F�X�@�\��t�������Ă���������, �̂Ă�̂����������Ȃ��A�v���ɂȂ����̂�,
���O���gTimer Precision�h�̗��������Ƃ������Ƃɂ���, �ۑ��łƂ��܂���. )</sub>

�v�̓^�C�}�[�̃u������m�F������������ł�.
�����̗���Ō��ʂ��o����Ă�����̂��炢�̂�, ���o�I�ɔ���₷���O���t�\���ɂ��Ă݂���ł�.

�{�A�v����, ��x�d�|�������� ( �J��Ԃ� ) �^�C�}�[�̃u�����v��������̂ł�.
�s�x�^�C�}�[���d�|�������Ĉ������v������悤�Ȃ��Ƃ͂��Ă��܂���.
Windows �̃^�C�}�[���ꎩ�̂̐��x���������̂ł͂Ȃ�,
�^�C�}�[�����S�̂�ʂ��Ẵu����������������ł�.

## �^�p

�^�C�}�[�l��I��� `Start` �����,

| �� | �Ӗ� |
| --- | --- |
| ���� | ���ۂɃ^�C�}�[����������(�ƃA�v�����F������)�o�ߎ��� ( 1pixel �� 1[ms] �̕���\ ) | 
| �c�� | ���ۂɃ^�C�}�[����������(�ƃA�v�����F������)���v�� ( 1pixel �� 1[��] ) | 

�Ƃ����O���t���X�V���Ă����܂�.

�v��[�q�X�g�O����](https://ja.wikipedia.org/wiki/�q�X�g�O����)�Ȃ�ł���,
�O���t�̏c���͑傫���Ȃ�, �c�_�̍��������ɂ߂悤�Ƃ����m���ł͂���܂���.
�ނ���[�N���}�g�O���t�B�[](https://ja.wikipedia.org/wiki/�N���}�g�O���t�B�[)�̂悤��,
�ǂ��ɂǂꂾ���̏c�_�������邩�����悤�Ƃ����m���ł�.

���Ԃ̕���\�� [ms] �P�ʂɊۂ߂Ă��܂�. ���� TP �Ƃ��Ă͂���Ȃ���ŏ\���Ȃ̂�.
<sub>( ���������\�[�X�����׍H�����[��S] �P�ʂł̃O���t���`��,
����͂���ŋ����[���G���������Ă����肷��̂ł���,
�����ł̖ړI�͂����܂� [ms] �P�ʂƂ��Ă��܂�. )</sub>

��, �v���̏��������\�̂悤�ɐݒ�ł��܂�.

| ���� | �Ӗ� |
| --- | --- |
| `Time:` | �^�C�}�[�l / [ms] �P�� | 
| `SetTimer` | [`SetTimer`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer) �̃u�����v��. | 
| `SetWaitableTimer` | [`SetWaitableTimer`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-setwaitabletimer) �̃u�����v��. | 
| `Period:` | [`timeBeginPeriod`](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod) �ɓn������\�l. 0 �Ŗ���. |
| `with` | `SetWaitableTimer` �̃^�C�}������ʒm�������. `WM_TIMER`, `WM_APP`, `Callback` �� 3���. |

��������ɂ����̂�, `SetWaitableTimer` �̂Ƃ��ɑI�ׂ�ʒm�����̑I�����ł�����.
`SetWaitableTimer`���g�p�����ꍇ, �^�C�}���������o����̂̓��C���X���b�h�Ƃ͕ʌ��̐�p�X���b�h�ƂȂ�܂�.
�����őI�ׂ�I������, ���C���X���b�h�փ^�C�}������m�点���i�Ƃ���, �ȉ��̑I������������Ƃ������Ƃł�.

1. �^�C�}�[�X���b�h���烁�C���X���b�h�� `WM_TIMER` �� [`PostMessage`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postmessagew).
1. �^�C�}�[�X���b�h���烁�C���X���b�h�� `WM_APP` �� [`PostMessage`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postmessagew).
1. �^�C�}�[�X���b�h���烁�C���X���b�h�̃R�[���o�b�N�֐����Ă�.

[`SendMessage`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendmessagew) ���g���I�����͗p�ӂ��Ă���܂���.
�u����g�����炢��������R�[���o�b�N�Ăׂ�v�Ƃ��������ł�.
�Ƃ�����, `SendMessage` ���Ă��܂���,
���C���X���b�h�����̎�̏���������������܂Ń^�C�}�[�X���b�h�̐i�s���~�܂�̂�,
�v���l�������₷����ł�.

����, ���� [`PostMessage`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postmessagew) ���g���̂�,
�Ȃ�Ł@`WM_TIMER` �� `WM_APP` �Ƃ�����̑I�������p�ӂ���Ă���̂��Ƃ�����,
[`WM_TIMER`](https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-timer) ���b�Z�[�W�͈����̗D��x���Ⴂ
( [���b�Z�[�W�L���[�ɑ��̃��b�Z�[�W���c���Ă��邤���͓͂��Ȃ�](
https://learn.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues#queued-messages) )
�Ƃ����� Windows �̎d�l�̉e���������قǂ̂��̂����Ă݂�����������ł�.

��, �v��������Ƀ^�C�}�[�����ݒ�̉��ɕ\���̂�, ���L�̐��l�ł�.

| ���� | �Ӗ� |
| --- | --- |
| `Min:` | �^�C�}�[����(�ƃA�v�����F������)���Ԃ̍ŏ��l ( [ms] �\�L�ŏ����_�ȉ�3�ʂ܂ł� [��S] �P�� ) | 
| `Ave:` | �^�C�}�[����(�ƃA�v�����F������)���Ԃ̕��ϒl ( [ms] �\�L�ŏ����_�ȉ�3�ʂ܂ł� [��S] �P�� ) | 
| `Max:` | �^�C�}�[����(�ƃA�v�����F������)���Ԃ̍ő�l ( [ms] �\�L�ŏ����_�ȉ�3�ʂ܂ł� [��S] �P�� ) | 
| `CPU:` | �v���������ɖ{�A�v������L���� CPU �g�p�� | 

CPU �g�p����, �I�}�P�̍��ڂ݂����Ȃ���ł���, 
[`timeBeginPeriod`](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod)
���g���� CPU �g�p���������Ƃ����b�𕷂����̂�, �e���������قǂ����Ă݂���������ł�.

�����Ă����΂񉺂ɂ���̂�, �v���𑀂鉺�\�̂��̂ł�.

| ���� | �Ӗ� |
| --- | --- |
| `Test:` | �e�X�g����. �w�肳�ꂽ���� ( [��] �P�� ) �Ōv�����I��. `0` �Ȃ� `Stop` ����܂Ōp��. |
| `Copy` | �e�X�g���ʂ� copy. �������Ȃǂ� paste ���ė��p�\. |
| `Stop` | �e�X�g�̏I��. |
| `Start` | �e�X�g�̊J�n. |

�e�X�g���Ԃ��w��ł���悤�ɂ����̂�, �قȂ� PC �œ������ԃe�X�g������, ���� PC ��ł�������ς��ăe�X�g�����肵��,
���ʂ�����ׂ�Ƃ��ɕ֗�������ł�.
�u���߂�1���͔S���v�Ƃ�����|��, [��] �P�ʂ̎w��Ƃ��Ă��܂�.

`Copy` �ŃN���b�v�{�[�h�ɓ���f�[�^��, �e�X�g�����ƃe�X�g���ʂ̃T�}���[�� 1�s��,
2�s�ڈȍ~�͂ǂ̎��Ԏ��ɉ���������������������̗���ł�.
2�s�ڈȉ��� Excel �� paste ����, Excel �ɃO���t��`������̂���ł���,
���̏ꍇ�́u�U�z�}�v( scatter chart ) �ŕ`�����Ă�������.
1��������Ă��Ȃ����Ԏ��̃f�[�^�͏ȗ����Ă���̂�, �_�O���t ( bar chart ) �ɂ͌����܂���.

������I��� `Start` �{�^������������, �v�����̕\���͉��}�̂悤�ɂȂ�܂�.

<picture>
  <img alt="Testing" src="Examples/Testing.png">
</picture>

CPU �g�p�����o��\��̉ӏ��� `Core` �Əo�Ă��܂���,
����̓^�C�}�[������ʒm����ē�����������, �ǂ̃R�A ( �_���v���Z�b�T�[ ) ��ŉғ����Ă��邩��\�����Ă��܂�.
`SetTimer` �ł���Ă���Ƃ��̃R�A�̔ԍ�������ւ��̂������邱�Ƃ�����܂�.
Windows �� `SetTimer` �̒ʒm������, ����ȕ��ɓ��藐�ꂽ�肷���ł���.
���Ȃ݂�, `SetWaitableTimer` �ł���Ă���ꍇ��, �v�����̃R�A�̓���ւ��͂���܂���.
�v���J�n���ɍ��ꂽ�X���b�h��, ��т��ĒS���𑱂��邩��ł�.

����, �u�Ȃ�� Core 2 �ł��񂾂�I Core 15 �Ƃ�, �����ƃq�}�����Ȃ�����邾��I�v�ȂǂƊϋq�Ȃ��烄�W���΂������Ȃ�̂�,
������̕����ł������ł�. ( �ǂ̃R�A���q�}�����Ȃ̂���,
[Resource Monitor](https://en.wikipedia.org/wiki/Resource_Monitor) �� CPU �^�u�ł��m�F��������. )
�ǂ̃R�A���g���̂��� Windows �̍єz����ɂȂ��Ă���, `Start` ���������тɏo��ڂ��Ⴄ���ƂɂȂ�܂�.
�u�R�A�K�`���v�Ƃł��Ăт����Ȃ�悤�ȏ󋵂ł�.
�Z�����ғ����Ă���v���Z�X���������Ă���R�A�����܂��ꂽ�ꍇ��, �v�����ʂ̐����������Ȃ�܂�.
( ���Ƃ�����, [`SetThreadAffinityMask`](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setthreadaffinitymask) �Ƃ�����̂�,
���ۂ̃A�v���̓���Ƃ��Ă̓��A���e�B�[�Ɍ����邵. )

�Ƃ������Ƃ�, �{�A�v�����e���o���v�����ʂ�, �e�X�g���邽�тɔ����� ( �܂�ɑ啝�� ) �قȂ�ꍇ������܂�.
������x�̃e�X�g���Ԃł�����x�̃e�X�g�񐔂��d�˂Ȃ���, �X�������؂����Ƃ͔��f�ł��܂���.

Windows �̒u���ꂽ�󋵂ɂ���Ă��v�����ʂ̓u���܂�.
�E�F�u�̓���𒭂߂Ă���u���E�U�������肷���, ���Ȃ萔���͈����Ȃ�܂�.
PC �̐��\�������ɋ����܂�. �R�A���̑����v���Z�b�T�[���L���ł�.
���ڃ����������Ȃ��� HDD ( SSD ) �� swap out ��������悤�� PC �͘_�O�ł�.

���̃u���������ł����炷����, `Test:` �Ńe�X�g���Ԃ��w�肵���ꍇ��, ���L�̂悤�Ɍv�����̕\���X�V���T�{��܂�.

<picture>
  <img alt="Testing" src="Examples/Testing2.png">
</picture>

�܂�, �C�x�ߒ��x�̂��̂�������܂���,
�v���ȊO�̏�������؃T�{��̂�, �v�����ʂ���萳�m�ɂȂ�悤�ȋC�����܂�.

����, ���̃��[�h�̏ꍇ, �e�X�g�I�����ɂ��m�点�̉�����܂�.

## ��

* Visual Studio 2022&reg; �ł� Windows11 ���� build �Ȃ̂�, 64bit OS �p�ł�.
* ���� build �� Windows10 �ł����Ȃ����삵�܂���, 64bit �łɌ���܂�.

## ����

`Start` �{�^����������,
�_�C�A���O��Őݒ肳�ꂽ�p�����[�^�[�������W�߂�, �w�肳�ꂽ���̃^�C�}�[���N�����܂�.

�^�C�}�[�̌v��������,

1. �u���݂̃J�E���g�l�v��[`QueryPerformanceCounter`](https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter)�œ���.
1. �u�O��̃J�E���g�l�v�Ƃ̍������u�o�ߎ��ԁv�Ƃ���.
1. �u�o�ߎ��ԁv�Ɋ�Â��čő�l�E�ŏ��l�̍X�V���s��.
1. �u�o�ߎ��ԁv��1[ms] �P�ʂɎl�̌ܓ��������l���C���f�b�N�X�Ƃ���, �O���t�p�̃J�E���^�[�z��� +1 ����.
1. �u���݂̃J�E���g�l�v������Ɍ����āu�O��̃J�E���g�l�v�Ɋi�[���Ă���.

�Ƃ����y�������������s���܂�.

��, `Test:` �Ńe�X�g���Ԃ��w�肵�Ă��Ȃ��ꍇ,
���̌v�������Ƃ͕ʌ��� 1[s] �� 1[��],
�ő�l�E�ŏ��l�E���ϒl��, �O���t�̕\�����X�V���܂�.
�����葁���X�������݂����ꍇ��, �������̕�������y�ł��傤.

���, `Test:` �Ńe�X�g���Ԃ��w�肵���ꍇ��,
�\�����T�{���Čv���݂̂ɐ�O���܂�.
���̏�ԂŎ莝���� PC ���v�����Ă݂�[����](Examples/README.md)���܂Ƃ߂Ă����܂���.

## �ǋL

�u[`timeBeginPeriod`](https://learn.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timebeginperiod)
�̉e���ɂ��Ă͒��ׂ���悤�����A
[`NtSetTimerResolution`](https://www.google.com/search?q=NtSetTimerResolution)
�ɂ��ẮH�v<br>
�Ƃ����^�� (�s��) �������ꂽ�������������邩������܂���,

[`winmm.dll` �� `timeBeginPeriod` ���ĂԂ� `ntdll.dll` �� `NtSetTimerResolution` ���Ăԍ\���ɂȂ��Ă���](https://mirrors.arcadecontrols.com/www.sysinternals.com/Information/HighResolutionTimers.html)

�����Ȃ̂�, ��{�I�ɗ��҂͓������̂ł�.
`NtSetTimerResolution` �𒼐ڌĂ񂾕���, �ق�̂�����҂�ׂ����w�� ( 0.5[ms] �P�� ) ���ł���̂ł���,
�����ł́u1[ms]�P�ʂł��イ�Ԃ�B�v�Ƃ����X�^���X�Œ������Ă���̂�,
�����͂Ȃ�������ł�.

�u[`CreateTimerQueueTimer`](https://learn.microsoft.com/en-us/windows/win32/api/threadpoollegacyapiset/nf-threadpoollegacyapiset-createtimerqueuetimer)
�́H�v<br>
[`SetWaitableTimer`](https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-setwaitabletimer) ��
�ړI�͉ʂ����Ă��܂����̂�, ���������ł�. ( �Ȃ񂩁glegacy�h�Ƃ��w�b�_�[���ɂ��邵. )

�u[`MMCSS`](https://learn.microsoft.com/en-us/windows/win32/procthread/multimedia-class-scheduler-service)�́H�v<br>
[�ړI�̃A�v��](../Bulletin/README.md) ���X�P�W���[�����O���������Ă܂ŗD������قǂ̂��̂ł͂Ȃ��̂�, �{ TP �Ƃ��Ă͎���o���Ă��܂���.
���̕ӂ̉e���͂����Ă݂�������, �������Ń\�[�X���������ċ@�\��ǉ����Ă݂Ă�������.


<div style="text-align: right;">
In-house Tool / �ƒ� �O
</div>
