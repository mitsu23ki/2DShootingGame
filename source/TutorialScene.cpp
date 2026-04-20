#include "TutorialScene.h"
#include "DxLib.h"
#include "AudioManager.h"
#include "CheckKey.h"

// コンストラクタ：各メンバの初期化
TutorialScene::TutorialScene()
    : camera(1280, 720),
    step(Step::Move),
    nextStep(Step::Move),
    itemPhase(ItemPhase::Heal),
    messageTimer(0),
    prevHP(0),
    nextScene(SceneType::None),
    canShoot(false),
    isStepComplete(false),
    isWaitingNext(false),
    isFading(false),
    enemyKilled(false)
{
}

// 初期化：シーン開始時のステートリセット
void TutorialScene::Init()
{
    player.Init();
    enemyManager.Init();
    itemManager.Init();

    canShoot = false;
    isFading = false;
    isStepComplete = false;
    isWaitingNext = false;
    messageTimer = 0;
    step = Step::Move;
    itemPhase = ItemPhase::Heal;
    enemyKilled = false;
    fade.SetFadeIn(500);
    AudioManager::PlayBGM(BGMType::Game);
}

// メイン更新：カメラ、プレイヤー、各ステップのロジック実行
void TutorialScene::Update()
{
    int camX = (int)camera.GetX();
    int camY = (int)camera.GetY();

    player.Update(camX, camY);
    camera.Update((int)player.GetX(), (int)player.GetY());

    messageTimer++;

    // 現在のステップに応じた更新処理の分岐
    switch (step)
    {
    case Step::Move:      UpdateMove(); break;
    case Step::Attack:    UpdateAttack(); break;
    case Step::Damage:    UpdateDamage(); break;
    case Step::ItemHeal:  UpdateItemHeal(); break;
    case Step::ItemPower: UpdateItemPower(); break;
    case Step::End:       UpdateEnd(); break;
    }

    int deltaTime = 16;
    fade.Update(deltaTime);

    if (isFading && fade.IsFinished())
    {
        ChangeStep(nextStep);
        fade.SetFadeIn(500);
        isFading = false;
    }

    // ステップ完了後の入力待ち（Enterキーで次へ）
    if (isWaitingNext && (CheckDownKey(KEY_INPUT_SPACE) || CheckDownKey(KEY_INPUT_RETURN)))
    {
        isWaitingNext = false;
        isStepComplete = false;
        isFading = true;
        fade.SetFadeOut(500);
    }

    // Fキーでメニュー画面
    if (CheckDownKey(KEY_INPUT_F))
    {
        nextScene = SceneType::Menu;
    }

    // Tキーでタイトル画面
    if (CheckDownKey(KEY_INPUT_T))
    {
        nextScene = SceneType::Title;
    }

    // Gキーでタイトル画面
    if (CheckDownKey(KEY_INPUT_G)) {

        nextScene = SceneType::HowToPlay;
    }
}

// 移動の練習
void TutorialScene::UpdateMove()
{
    if (player.HasMoved()) // プレイヤーが移動を検知したら完了
    {
        isStepComplete = true;
        isWaitingNext = true;
        nextStep = Step::Attack;
    }
}

// 攻撃の練習
void TutorialScene::UpdateAttack()
{
    enemyManager.Update(player.GetX(), player.GetY(), player.GetRadius(), player, dummyScore);

    auto& enemies = enemyManager.GetEnemies();
    if (!enemies.empty()) enemies[0].SetCanShoot(false); // 攻撃練習中は敵を撃たせない

    if (canShoot) laserSystem.Update(player, enemyManager);

    if (messageTimer < 30) return;

    for (const auto& e : enemies)
    {
        if (e.IsDying()) // 敵を撃破したら完了
        {
            isStepComplete = true;
            isWaitingNext = true;
            nextStep = Step::Damage;
            break;
        }
    }
}

// ダメージ体験
void TutorialScene::UpdateDamage()
{
    auto& enemies = enemyManager.GetEnemies();
    if (!enemies.empty())
    {
        if (player.HasMoved())
        {
            enemies[0].SetCanShoot(true); // 動いたら撃つ
        }
        else
        {
            enemies[0].SetCanShoot(false); // 動くまで撃たない
        }
    }

    if (messageTimer == 1) player.SetInvincible(0, false);

    enemyManager.Update(player.GetX(), player.GetY(), player.GetRadius(), player, dummyScore);

    if (player.GetHP() < PlayerConst::MAX_HP) // HPが減ったら完了
    {
        isStepComplete = true;
        isWaitingNext = true;
        nextStep = Step::ItemHeal;
    }
}

// HP回復
void TutorialScene::UpdateItemHeal()
{
    itemManager.Update(player);

    if (player.GetHP() > prevHP && !isStepComplete) // HPが増加（回復）したら完了
    {
        isStepComplete = true;
        isWaitingNext = true;
        nextStep = Step::ItemPower;
    }

    prevHP = player.GetHP();
}

// 無敵
void TutorialScene::UpdateItemPower()
{
    itemManager.Update(player);

    if (messageTimer == 1)
    {
        itemManager.SpawnAt(player.GetX() + 100, player.GetY(), ItemType::Power);
    }

    if (player.IsInvincible() && player.HasMoved() && !isStepComplete) // 無敵化したら完了
    {
        isStepComplete = true;
        isWaitingNext = true;
        nextStep = Step::End;
    }
}

// ステップの切り替え：フラグのリセットと次のステップの準備
void TutorialScene::ChangeStep(Step next)
{
    isStepComplete = false;
    isWaitingNext = false;
    step = next;
    messageTimer = 0;

    enemyManager.Clear();
    itemManager.Init();
    player.ResetForTutorial();
    player.ResetInputFlag();
    AudioManager::PlaySE(SEType::Decide);

    // 次のステップに応じた固有の初期化
    switch (next)
    {
    case Step::Attack:
        canShoot = true;
        enemyManager.SpawnOneFake(player.GetX() + 300, player.GetY());
        break;
    case Step::Damage:
        canShoot = false;
        enemyManager.SpawnOneFake(player.GetX() + 300, player.GetY());
        break;
    case Step::ItemHeal:
        itemPhase = ItemPhase::Heal;
        itemManager.SpawnAt(player.GetX() + 100, player.GetY(), ItemType::Heal);
        player.OnDamage(); // ダメージ状態にしておく
        prevHP = player.GetHP();
        break;
    case Step::ItemPower:
        itemPhase = ItemPhase::Power;
        break;
    default:
        break;
    }
}

// Step: End 
void TutorialScene::UpdateEnd()
{
    if (messageTimer > 60 && CheckDownKey(KEY_INPUT_SPACE))
    {
        nextScene = SceneType::Game; // 本編へ遷移
    }
}

void TutorialScene::Draw()
{
    // 背景とゲーム内オブジェクトの描画 
    DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);

    int camX = (int)camera.GetX();
    int camY = (int)camera.GetY();

    enemyManager.Draw(camX, camY);
    if (canShoot) laserSystem.Draw(player, camX, camY); // 射撃許可時のみ描画
    player.Draw(camX, camY);
    itemManager.Draw(camX, camY);

    // UI表示用の色の定義
    int uiBg = GetColor(60, 140, 60);    // UI背景（濃緑）
    int white = GetColor(255, 255, 255);  // メイン文字（白）
    int accent = GetColor(255, 255, 180);  // 強調テキスト（薄黄）

    // 操作ガイドの描画（画面左下)
   // DrawBox(0, Screen::HEIGHT - 80, 300, Screen::HEIGHT, uiBg, TRUE);
    DrawString(20, Screen::HEIGHT - 85, "Gキー：操作説明", white);
    DrawString(20, Screen::HEIGHT - 60, "Tキー：タイトルへ", white);
    DrawString(20, Screen::HEIGHT - 35, "Fキー：メニューへ", white);
   

    // 現在のステップに応じた説明テキスト
    if (!isFading && !isStepComplete)
    {
        DrawBox(0, 0, 1280, 90, uiBg, TRUE);

        if (step == Step::Move)
        {
            DrawString(100, 20, "クリックで移動しよう", white);
            DrawString(100, 50, "行きたい場所をクリックすると移動するよ", white);
        }
        else if (step == Step::Attack)
        {
            DrawString(100, 20, "敵に近づくとレーザー攻撃できるよ", white);
            DrawString(100, 50, "レーザーは自動で発射されるよ", white);
        }
        else if (step == Step::Damage)
        {
            DrawString(100, 20, "敵の黄色の弾に当たってみよう", white);
            DrawString(100, 50, "弾や敵に当たるとHPが減るよ", white);
            
        }
        else if (step == Step::ItemHeal)
        {
            
            DrawString(100, 20, "回復アイテムを取ろう", white);
            DrawString(100, 50, "HPが回復するよ", white);
        }
        else if (step == Step::ItemPower)
        {
            
            DrawString(100, 20, "無敵アイテムを取ろう", white);
            DrawString(100, 50, "一定時間攻撃が効かなくなるよ", white);
        }
    }

    // 終了案内表示
    if (step == Step::End)
    {
        int cx = Screen::WIDTH / 2;
        int cy = Screen::HEIGHT / 2;

        DrawBox(cx - 220, cy - 100, cx + 220, cy + 100, uiBg, TRUE);
        DrawString(cx - 100, cy - 60, "チュートリアル完了", white);
        DrawString(cx - 140, cy - 20, "SPACE：ゲーム開始", white);
        DrawString(cx - 140, cy + 20, "T：タイトル", white);
        DrawString(cx - 140, cy + 50, "F：メニュー", white);
    }

    // 課題達成時のメッセージ表示
    if (isStepComplete)
    {
        DrawBox(0, 0, 1280, 90, uiBg, TRUE);
        DrawString(520, 30, "達成", accent);
        DrawString(450, 55, "SPACE 次へ", white);
    }

    // シーン遷移用の暗転フェード描画
    fade.Draw();
}

// 次シーン情報の取得
SceneType TutorialScene::GetNextScene() const
{
    return nextScene;
}