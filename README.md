# 简介
  基于 UE5.5 C++ 开发的综合型 3D 动作战斗项目。独立完成战斗系统，运动系统，动画，AI 行为，资源管理及工具链等多模块开发。围绕 Gameplay Ability System 构建角色能力与战斗框架、
  扩展 CharacterMovementComponent 实现了攀爬等自定义移动模式、结合 ControlRig，MotionWarping 实现流畅动画及运动表现、
  子类化 UAssetManager 实现软引用资源管理器、EQS 搭建复杂 AI、使用 Slate 开发对编辑器进行功能扩展。项目约 90% 逻辑使用 c++ 实现，蓝图占比低。

# 目录
  

# 战斗系统
## 近战
  近战使用武士刀进行攻击。
  ### 普通攻击连段
  攻击有四连段，在前一段攻击的后摇阶段中再按攻击键会触发下一个连段，可触发连段的后摇时机在 Montage 中使用动画通知进行配置，连段的触发和调度逻辑由 Ability 控制。
  
  还使用了 MontionWarping 进行攻击吸附，在近距离攻击时会将角色吸附到锁定目标的固定距离前。

  <img src="ReadmeFile/Gif/KatanaAttack.gif">

  ### 格挡反击
  使用输入触发 [格挡 Ability](Source/DX_InfiniteCombat/Private/GAS/GA/GA_Block.cpp)，播放 Montage，Ability 持续期间若角色前向受击，则免疫该伤害且对敌人进行反击。

  <img src="ReadmeFile/Gif/Block1.gif">
  
  ### 处决
  当敌人生命值到达斩杀线且 [GA_Execution](Source/DX_InfiniteCombat/Private/GAS/GA/GA_Execution.cpp) 的 CanActivateAbility 为 true 后，将显示处决按键，按下对应按键后触发处决 Ability ,使用 MotionWarping 令处决动画精准命中处决目标，LevelSequence 进行运镜。
  
  <img src="ReadmeFile/Gif/Execution2.gif">
  <img src="ReadmeFile/Gif/Execution1.gif">
  
  ### 蓄力攻击
  长按攻击键到达一定时间后松开，使用 Ability Task 的 WaitInputReleased 监听按键释放事件，检测按下时间足够就触发蓄力攻击。

  <img src="ReadmeFile/Gif/KatanaCharge2.gif">

  ### 打击感/命中反馈
  当命中目标后调用 [AttackComponent](Source/DX_InfiniteCombat/Private/ICComponents/AttackComponent.cpp) 中的 HitFeedback 方法进行镜头抖动，短时的全局时间膨胀;\
  使用 Montage_SetPlayRate 对当前播放的攻击 Montage 进行缓速从而实现顿帧感；\
  可配置跳过当前播放的攻击 Montage 的一定帧数，从而实现抽帧效果；\
  伤害跳字，造成伤害时会跳出伤害数字，该跳字 UI 使用了对象池，避免频繁创建销毁，UI 对象池统一由一个[主 UI](Source/DX_InfiniteCombat/Private/UMG/WidgetCombatStates.cpp) 管理。
  
## 远程
  远程武器弓箭；\
  瞄准射击逻辑，将[时间轴封装到 c++ ](Source/DX_InfiniteCombat/Public/Utils/TimelineUtils.h)中，时间轴驱动[瞄准视角的切换](Source/DX_InfiniteCombat/Private/Character/DXCharacterExtensionComponent.cpp)；
  
  箭矢发射使用了 UE 的 UProjectileMovementComponent，实现了一个 [ProjectorActorBase](Source/DX_InfiniteCombat/Private/Projector/ProjectorActorBase.cpp) 作为所有发射物的基类，其中扩展了伤害判定，听觉感知发送，以及投射物命中后消失或附加到命中目标上的逻辑。
  
  <img src="ReadmeFile/Gif/LuanchArrow.gif">

## 伤害判定/应用
  在 [AttackComponent](Source/DX_InfiniteCombat/Private/ICComponents/AttackComponent.cpp) 中提供攻击检测逻辑，在动画通知中调用;\
  命中目标后 调用 [AttackComponent](Source/DX_InfiniteCombat/Private/ICComponents/AttackComponent.cpp) 中接口构造 GameplayEffectContext 并对命中目标施加伤害 GE DamageEffect，发送伤害感知;\
  DamageEffect 使用[自定义的 Execution calculation class](Source/DX_InfiniteCombat/Private/GAS/ICDamageExecution.cpp)计算伤害；\
  在自定义 Execution 中，获取了目标的防御力，免伤数值，以及发起者的攻击力，然后使用了[自定义的 FGameplayEffectContext](Source/DX_InfiniteCombat/Public/GAS/ICGameplayEffectTypes.h)来传递本次伤害系数，在攻击的 Montage 动画通知状态中配置数值，便于为每一段攻击配置不同的伤害系数。

  ### 受击
  DamageEffect 中触发一个 [受击 GameplayCue](Source/DX_InfiniteCombat/Private/GAS/GC_AttakHit.cpp), 在 GameplayCue 中触发伤害跳字，命中音效以及播放受击 Montage；\
  根据不同受击方向会播放不同 Montage ，受击方向由命中点法线和命中目标的前向和右向向量夹角大小计算得出。

  后向
  
  <img src="ReadmeFile/Gif/GetHitBack.gif">
  
  前向
  
  <img src="ReadmeFile/Gif/GetHitFront.gif">
  
  左向
  
  <img src="ReadmeFile/Gif/GetHitLeft.gif">
  
  右向
  
  <img src="ReadmeFile/Gif/GetHitRight.gif">
  
  ### 伤害免疫
  将 ASC 中的 OnImmunityBlockGameplayEffectDelegate 绑定到免疫事件，角色在冲刺时对自己施加一个免疫 DamageEffect 效果的 GE，在冲刺过程中受到伤害就会免疫伤害 GE 并触发免疫事件，用 PoseableMesh 制作定格残影，后处理材质实现色差效果。

  <img src="ReadmeFile/Gif/Dodge2.gif">
  <img src="ReadmeFile/Gif/Dodge3.gif">
  
# 锁定系统
  [锁定组件](Source/DX_InfiniteCombat/Private/LockSystem/WeakLockComponent.cpp)
  锁定策略偏软锁定；为避免限制玩家操作，不会强制将控制器完全锁定在目标中心处，只会保持一定的可视屏幕边距，在一定时间内没有再触发锁定操作会自动取消锁定；
  
  锁定到目标时，控制器仍然能够转动，只有在目标超出了屏幕范围，控制器才会平滑到锁定点离屏幕边缘 x 像素处，x 可配置，目前只在 [锁定组件](Source/DX_InfiniteCombat/Private/LockSystem/WeakLockComponent.cpp) 中配置一个值，但可改为每个锁定目标配置单独的值，从而在锁定不同体型敌人时保持不同的屏幕边距。

  <img src="ReadmeFile/Gif/WeakLock1.gif">
  <img src="ReadmeFile/Gif/WeakLock2.gif">

# GamplayAbilitySystem
  本项目使用了 UE 的 GAS 插件，除了基础移动输入其他主要行为触发基本都由 Ability 实现，主要是运用其 GamePlayTag 就能轻松配置各种行为之间的互斥，打断，阻挡关系，GE 对属性的修改，Debuff 施加也十分方便，但介于本项目是单机，对 GAS 网络同步方面的研究较浅；
  
  主要的 Ability C++ 类 在[GA文件夹](Source/DX_InfiniteCombat/Private/GAS/GA)下，还有一些 Ability 是纯蓝图；
  
  此外还[继承了 ASC ](Source/DX_InfiniteCombat/Private/GAS/ICAbilitySystemComponent.cpp)，对能力系统组件进行了扩展，主要是输入与 Ability 的绑定，让某些输入通过配置就能直接触发只能 Ability，然后还有一些对 ActivateAbility 和 SetByCaller 的封装。
  
# 运动系统
## 攀上障碍
  在[Traversal Ability](Source/DX_InfiniteCombat/Private/GAS/GA/GA_Traversal.cpp) 中，使用胶囊体扫描，模拟角色攀登路径，确认碰撞条件满足，再判断落点是否满足可站立条件，都满足后触发 Montage 播放；\
  使用 MotionWarping ，实现了同一个根运动动画能够精确攀上不同高度障碍物的效果。

  <img src="ReadmeFile/Gif/Traversal1.gif">
  <img src="ReadmeFile/Gif/Traversal3.gif">
  
## 攀爬墙体
  [扩展移动组件](Source/DX_InfiniteCombat/Private/ICComponents/ICCharacterMovementComponent.cpp)，重写 PhysCustom 扩展了新的移动模式 PhysClimbing，沿用了 PhysWalking 的分步模拟思路，每个子步中, FindAndUpdateClimbSurface() 分部位进行多个球形探测，对多个命中结果进行筛选和加权平均获取墙面信息；
  
  UpdateClimbingAcceleration() 中对获取到的墙面法线进行投影和叉乘，获得墙面坐标系，然后用点积的方式获取加速度在移动输入前向和右向的投影长度，将其分别乘到墙面坐标系的上方向向量和右方向向量， 最后将这两个向量相加得到最新的加速度，由 CalcVelocity 由加速度和摩擦力等计算出速度后，在 ClimbAlongSurface() 中将速度转换为实际的位移 MoveDelta，SafeMoveUpdatedComponent 做实际的移动；
  
  当攀爬到墙壁顶部时，上半身的球形探测未命中，下半身球形探测命中，此时将尝试攀上墙顶，攀上墙顶则是复用的上面的攀上障碍逻辑。

  <img src="ReadmeFile/Gif/ClimbTop3Debug.gif">
  <img src="ReadmeFile/Gif/ClimbTop1.gif">

# 动画表现
## IK
  本项目 IK 都使用 [ControlRig](Content/Character/Assets/Samurai2/Character/CtrlRig_Samurai.uasset) 实现，主要使用了 BasicIK 节点，脚踝旋转则使用了 Aim 节点，对各复杂地形贴合效果都很好。

  <img src="ReadmeFile/Images/ControlRig_FootIk.png">
  
  <img src="ReadmeFile/Images/ControlRig_Aim.png">
  
  ### 地面脚部 IK
  <img src="ReadmeFile/Gif/FootIk.gif">\
  <img src="ReadmeFile/Gif/GroundIK.gif">
  
  ### 攀爬时的手部 IK 和 脚部 IK
  攀爬时脚部 IK 是从 Y 方向(骨骼空间前向)进行的检测，并且因为是用脚尖抵墙，所以需要加上 Foot 骨骼和 Ball 骨骼的距离的长度偏移，同理手部也会加上手指到 Hand 骨骼的距离偏移。

  <img src="ReadmeFile/Images/ControlRig_ClimbDebug.png">
  
  <img src="ReadmeFile/Gif/Climb1.gif">
  
## 基础运动
  移动数值，状态的更新都使用了动画多线程 BlueprintThreadSafeUpdateAnimation 更新，减少游戏线程开销；

  使用同步标记对动画切换时的左右脚进行同步，动画差异大时使用惯性化过渡，攀爬墙面和弓箭瞄准都使用了混合空间，对于一些半身 Montage 动画，使用了Layered blend per bone 进行分层混合。

  <img src="ReadmeFile/Images/BS_Climb.png">
  
  <img src="ReadmeFile/Images/ABP_UpperBody.png">

  原地转向；

## 逻辑复用
  使用了 LinkedAnimLayer 和 动画层接口 AnimLayerInterface ，对同一骨骼的动画逻辑进行复用，且可在运行时动态切换，详情见 [ALIBase](Content/Character/BP/ALI/ABP_ALIBase.uasset) 和 [ALI_Character](Content/Character/BP/ALI/ALI_Character.uasset) ，
  
  对于同一个骨骼，当需要替换成另一套动画表现而不需要改变 AnimGraph 逻辑时，只需创建一个 [ALIBase](Content/Character/BP/ALI/ABP_ALIBase.uasset) 的子类，然后更改动画序列成员默认值，无需将整个动画逻辑重写，运行时，使用 LinkAnimClassLayers 进行切换；

  <img src="ReadmeFile/Images/ALI.png">

  而对于不同骨骼之间的动画蓝图逻辑复用则使用[动画蓝图模板](Content/Character/EnemyBP/ABPI_EnemyBase.uasset)，在各自的动画图表中，使用 LinkedAnimGraph 链接到同一个模板；

  <img src="ReadmeFile/Images/ABPI.png">

## 动画重定向
  不同骨骼的动画序列资源复用；
  
  项目使用的骨骼网格体资源包里只附带几个基础动画，其他的战斗，攀爬，弓箭，武士刀等等动画资源都是用的其他多个动画资源集合，从各个骨骼，使用 IK Retargeter 的骨骼链映射重定向到这一个骨骼网格体中。
  
  <img src="ReadmeFile/Gif/Retarget.gif">

# 软引用资源管理
  本项目参考了 Lyra 示例项目，子类化 UAssetManager，实现了[自己的 AssetManager](Source/DX_InfiniteCombat/Public/Data/ICAssetManager.h)，对一些非常态加载，以及能接受一定延迟的资源，实现动态加卸载，以及使用软引用的方式，避免过多的硬引用，只在必要时加载进内存；

  <img src="ReadmeFile/Images/AssetManager.png">

  # AI
  AI 行为使用了常规的行为树，黑板架构，创建了一些自定义的行为树任务 BTTask，还使用了感知组件，主要使用了痛觉，视觉，听觉，应用伤害时使用 UAISense_Damage::ReportDamageEvent 向目标发送痛觉感知；

  在 [自定义的 AIController](Source/DX_InfiniteCombat/Public/AI/ICAIController.h) 中重写了 GetGenericTeamId() 来对 AI 单位进行团队分组，并对黑板键和感知的更新做了封装；
  
  此外为了构建更智能的 AI 行为，还使用了 EQS 环境查询系统，构建了环境查询树，
  
  以令 AI 寻找一个合适的移动目标点为例，使用 查询生成器(EnvQueryGenerator) 在 查询情景(EnvQueryContext) 周围生成规则点，根据查询测试(EnvQueryTest)，为这些点打分或过滤，选择得分最高的点或者得分前 5%/25% 中的随机一点，在行为树中将其更新到黑板键里，作为 AI 最终的移动目标位置。
  
  <img src="ReadmeFile/Images/EQS1.png">

  <img src="ReadmeFile/Images/EQS2.png">

  # 编辑器扩展
  使用 Slate 编程，实现了一个编辑器扩展模块 [EditorExtension](Source/EditorExtension/Private/EditorExtensionModule.cpp)，由于本项目没什么编辑器工具需求，这里只是简单实现了一个编辑器中英文快速切换的功能。

  <img src="ReadmeFile/Gif/EditorExtent.gif">

# 存档
  [存档管理器](Source/DX_InfiniteCombat/Public/SaveGameManager.h)管理存档的读取，自动保存，手动保存，版本转换；
  
  [存档索引](Source/DX_InfiniteCombat/Public/SaveIndex.h) SaveIndex 本身也是一个 USaveGame 文件，存放在一个固定槽位名下，它内部维护所有槽位的元数据，元数据中包含版本号，时间戳，显示名称，缩略图等。
  
# 用户自定义输入
  在输入映射上下文中重载设置，为按键配置一个 Name, 然后调用增强输入用户设置中的 MapPlayerKey ，根据对应的 Name 就能更改按键映射，最后使用 ApplySettings 应用更改，SaveSettings 持久化。

  <img src="ReadmeFile/Images/PlayerCusInput1.png">

  按键设置界面，UI 随便做的，比较简陋。
  
  <img src="ReadmeFile/Images/PlayerCusInput2.png">

# 物理破坏
  GeometryCollection 组件，物理场...
# 连招系统
  前缀树...
