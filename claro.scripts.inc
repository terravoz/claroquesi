<?php
// $Id$
/**
 * @file
 * Audio Blog VoIP Scripts and helper functions
 */

/**
 * Implementation of hook_voipscript_get_script_names()
 */
function claro_voipscript_get_script_names() {
  $script_names[] = 'claro_main_menu_script';
  $script_names[] = 'claro_welcome_script';
  $script_names[] = 'claro_sms_handler_script';
  return $script_names;
}

/**
 * Implementation of hook_voipscript_load_script()
 */
function claro_voipscript_load_script($script_name, $options=NULL) {

  $script = NULL;
  switch ($script_name) {
    default:
      break;
    case 'claro_welcome_script':
      $script = new VoipScript('pph_welcome_script');
      $script->addSetVoice('woman');
      $options['voice'] = 'claro';
      voipvoice_set_current_voice('claro');
      $script->addSet('max_session_duration', 0);

      // log the call
      $log_msg = t("Call from %caller_number on @date (id: %call_id)",
        array('@date' => format_date(time(), 'MDT')));
      $script->addLog($log_msg, 'claro');


      $script->addLabel('language_selection');
      /*$p_main_menu[] = v("Para Espanol, oprima uno. ", array(), NULL, 'carmen-twilio');
      $p_main_menu[] = v("Para Portugues, tecle dois. ",  array(), NULL, 'portuguese-brazil');*/
      $p_main_menu[] = v("Para Espanol, oprima uno. Para Portugues, tecle dois. For English, press three. ");
      $input_options = array(
      '1' => 'spanish_version',
      '2' => 'portugese_version',
      '3' => 'english_version',
      '#' => 'invalid_option',
      'i' => 'invalid_option',
      't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_main_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('spanish_version');
      $script->addGosub('claro_coming_soon', $options);
      $script->addGoto('language_selection');

      $script->addLabel('portugese_version');
      $script->addGosub('claro_coming_soon', $options);
      $script->addGoto('language_selection');


      $script->addLabel('english_version');
      //$options['voice'] = 'woman';
      $script->addGosub('claro_intro_script', $options);
      $script->addGoto('hang_up');

      $script->addLabel('invalid_option');
      $script->addGoto('hang_up');

      $script->addLabel('hang_up');
      $script->addHangup();
      break;
    case 'claro_intro_script':
      $script = new VoipScript('claro_intro_script');
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);
      $script->addSay(v("If this is an emergency, hang up and dial 9 1 1.
      Welcome to Convo with Clara! (Bienvenido a Claro Que Si), a telephone talk show for domestic workers."));
      $script->addGosub('claro_main_menu_script', $options);
      $script->addHangup();
      break;
    case 'claro_main_menu_script':
      $script = new VoipScript('claro_main_menu_script');
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);

      $script->addLabel('options_menu');
      $p_options_menu = v("Main Menu.
To sign up to to receive weekly text messages, press 1.
To hear humorous episodes about need-to-know essentials, press 2.
To learn about the Domestic Workers Bill of Rights in New York, press 3.
To learn more about Claro que Si, press 4.
To repeat this menu, press the star key.");

      $input_options = array(
        '1' => 'sms_subscription',
        '2' => 'play_episodes',
        '3' => 'domestic',
        '4' => 'get_information',
        '*' => 'repeat',
        '#' => 'hang_up',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('play_episodes');
      $script->addGosub('claro_play_episodes_demo', $options);
      $script->addGoto('options_menu');

      $script->addLabel('get_information');
      $script->addSay(v('Claro Que Si was produced by R E V an art, media, and
      social justice nonprofit. Visit w w w dot s t u d i o r e v dot o r g for more info.
      Our collaborators are M I T Center for Civic Media, Terravoz, NuLawLab,
      an initiative of Northeastern University School of Law,
      Brazilian Immigrant Center, and the National Domestic Workers Alliance.
Claro Que Si is made possible through the support of
Tribeca Film Institute, Rockefeller Foundation Cultural Innovation Fund,
Funding Exchange, Franklin Furnace, M I T Community Service Fund, and M I T Council for the Arts.
Find us online at w w w dot c l a r a s h o w dot c o m .'));
      $script->addGoto('options_menu');

      $script->addLabel('sms_subscription');
      $options['signup_number'] = '%caller_number';
      $script->addGosub('claro_sms_subscription_menu', $options);
      $script->addGoto('options_menu');

      $script->addLabel('domestic');
      $script->addSay(v('Your call will now be redirected to NDNS. '));
      //$script->addDial('+16466993989');
      $script->addHangup();

      $script->addLabel('repeat');
      $script->addGoto('options_menu');

      $script->addLabel('invalid_option');
      $script->addGoto('options_menu');

      $script->addReturn();
      break;
    case 'claro_play_episodes_demo':
      $script = new VoipScript('claro_play_episodes_demo');
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);

      $script->addLabel('options_menu');
      $p_options_menu = v("Episode directory.
To learn about cleaning products that can harm — or even kill — you, press 1.
Preventing bed bug infestation, press 2.
Staying safe in style, press 3.
History of domestic workers, press 4.
To repeat this menu, press the star key.
To go back to the main menu, press the pound key.
");

      $input_options = array(
        '1' => 'da_lungs',
        '2' => 'bed_bug',
        '3' => 'fashion',
        '4' => 'melnea',
        '*' => 'repeat',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('da_lungs');
      $options['story_id'] = 13;
      $script->addGosub('claro_play_story_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('bed_bug');
      $options['story_id'] = 42;
      $script->addGosub('claro_play_story_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('fashion');
      $options['story_id'] = 84;
      $script->addGosub('claro_play_story_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('melnea');
      $options['story_id'] = 85;
      $script->addGosub('claro_play_story_script', $options);
      $script->addGoto('options_menu');

      $script->addLabel('repeat');
      $script->addGoto('options_menu');

      $script->addLabel('go_back');
      $script->addReturn();
      break;

    case 'claro_play_story_script':
      $script = new VoipScript('claro_play_story_script');
      $story_id = $options['story_id'];
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);

      $episode = claro_get_episode($story_id);
      if(!$episode) {
        $script->addSay("Invalid story id. ");
        $script->addLog("Invalid story id: $story_id. ");
        $script->addGoto('go_back');
      }
      else {
        $prompt =  new VoipPrompt($episode['body']);
      }

      $script->addLabel('play_story');
      $p_episode_intro = v("You're about to listen to a new episode, but listen up.
If you want to start over, hear the list of episodes or the main menu, press any key at any time.");


      $p_episode_prompt[] = $p_episode_intro;
      $p_episode_prompt[] = $prompt;
      //$p_episode_prompt[] = $p_episode_outro;

      $script->addGetInput($p_episode_prompt, 1);

      $script->addGotoIf('episode_outro', "^%input_digits");

      $script->addLabel('episode_outro');
      $p_episode_outro =v('To repeat this episode, press 1.
      To hear other episodes press 2.
      To go back to the main menu, press the pound key. ');
      $input_options =  array(
        '1' => 'repeat',
        '2' => 'go_back',
        '#' => 'go_back_main_menu',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_episode_outro, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('repeat');
      $script->addGoto('play_story');

      $script->addLabel('go_back');
      $script->addReturn();

      //@todo: test
      $script->addLabel('go_back_main_menu');
      $script->addGosub('claro_main_menu_script', $options);
      $script->addHangup();
      break;

    case 'claro_coming_soon':
      $script = new VoipScript('claro_coming_soon');
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);
      $script->addSay(v('This part of the system is coming soon. '));
      $script->addReturn();
      break;
    case 'claro_sms_subscription_menu':
      $script = new VoipScript('claro_sms_subscription_menu');
      $voice =  $options['voice'];
      voipvoice_set_current_voice($voice);

      $signup_number = $options['signup_number'];
      $text = 'Welcome to Claro Que Sí! To subscribe, text the letter "J". To stop receiving text messages, text "S". For help, text "H". Msg & data rates may apply.';

      $script->addSendText($text, $signup_number);

      $script->addLabel('options_menu');
      $p_options_menu = v("Claro Que Si offers weekly text messages for nannies,
      housekeepers, and caregivers about health and safety, history, rights, and more.
You should have already received a text message with instructions to confirm your subscription.
So check your cell phone, and thanks for signing up!
To repeat this message, press the star key.
To go back to the main menu, press the pound key.
");

      $input_options = array(
        //'1' => 'confirm',
        '*' => 'repeat',
        '#' => 'go_back',
        'i' => 'invalid_option',
        't' => 'invalid_option'
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('repeat');
      $script->addGoto('options_menu');

      $script->addLabel('go_back');
      $script->addReturn();

      break;
    /*case 'claro_main_menu_script':
      //Audio blog script for Voice channel
      $script = new VoipScript('claro_main_menu_script');
      $script->addSetVoice('woman');
      voipvoice_set_current_voice('woman');

      $script->addSay(v('Welcome to Claro que Si.'));
      $script->addLabel('main_menu');
      $p_options_menu = v('Main menu. To listen to the latest episodes, press 1. ');

      $input_options = array(
        '1' => 'play_episodes',
      );
      $p_invalid_msg = v('Invalid option selected');
      $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
      $script->addGoto('%ivr_option_selected');

      $script->addLabel('play_episodes');
      $script->addGosub('claro_play_episodes_script', $options);
      $script->addGoto('main_menu');

      $script->addLabel('end');
      $script->addSay(v('Thank you for calling. Bye bye.'));
      $script->addHangup();
      break;

    case 'claro_play_episodes_script':
      //Helper script that provides play menu for Audio blogs.
      $script = new VoipScript('claro_play_episodes_script');
      $script->addSetVoice('woman');
      voipvoice_set_current_voice('woman');

      $episodes = _claro_get_episodes();
      if (!$episodes) {
        $script->addSay(v('There are no episodes at this time. '));
      }
      else {
        $script->addSay(v('During playback, press any key to go to the main menu. '));
        foreach ($episodes as $index => $episode) {
          $body = $episode['body'];
          //$changed = $episode['changed'];

          $args = array('@date' => VoipVoice::getDate($episode['changed']),);
          $info = v('This episode was aired on @date', $args);
          $current = "audio_blog_$index";
          $current_info = "info_$index";
          $next = 'audio_blog_' . ($index+1);

          $script->addLabel($current);
          $script->addGetInput($body, 1, '#', 3);

          $script->addLabel('playback_menu');
          $p_options_menu = v('To replay the current episode, press 1. To listen to the additional information about this episode press 2. To go to the next episode, press 3. To go back to the main menu, press the pound key. ');
          $input_options = array(
            '1' => $current,
            '2' => $current_info,
            '3' => $next,
            '#' => 'go_back',
          );
          $p_invalid_msg = v('Invalid option selected');
          $script->addRunIvrMenu($p_options_menu, $input_options, $p_invalid_msg);
          $script->addGoto('%ivr_option_selected');

          $script->addLabel($current_info);
          $script->addSay($info);
          $script->addGoto('playback_menu');
        }
      }

      $no_more = count($episodes);
      $script->addLabel("audio_blog_$no_more");
      $script->addSay(v('No more episodes to be played. '));
      $script->addGoto('go_back');

      $script->addLabel('invalid_option');
      $script->addGoto('go_back');

      $script->addLabel('go_back');
      $script->addReturn();
      break;
*/
    case 'claro_sms_handler_script':
      //Claro VoIP script for Text channel
      $script = new VoipScript('claro_sms_handler_script');
      // log the sms
      $log_msg = t("SMS from %caller_number on @date (id: %call_id)",
        array('@date' => format_date(time(), 'custom', 'D, m/d/Y - H:i:s')));
      $script->addLog($log_msg, 'MDT');

      // below was the only way to avoid problems with special characters in the text message
      $options['text'] = '%inbound_text_contents';
      $options['caller_number'] = '%caller_number';
      $options['dest_number'] = '%dest_number';
      $options['call_id'] = '%cid';
      $options['source'] = '%source';
      $script->addGosub('claro_sms_process_request_script', $options);
      break;
    case 'claro_sms_process_request_script':
      //Helper script to process SMS request
      $script = new VoipScript('claro_sms_process_request_script');

      $text = _claro_sms_process_request($options['text'], $options['caller_number'], $options['dest_number'], $options['call_id']);

      if($text) {
        //$footnote = variable_get('claro_sms_footnote', '');
        $script->addSendText($text);
        $script->addLog("sending $text");
      }

      $script->addHangup();
      break;
  }

  return $script;
}

/**
 * Process incoming SMS messages
 */
function _claro_sms_process_request($msg, $caller_number, $dest_number, $call_id) {
  watchdog('claro', "In _claro_sms_process_request with $msg, $caller_number, $dest_number");
  $caller_number = VoipCall::NormalizeNumber($caller_number);
  $help_msg = variable_get('claro_sms_help', 'Welcome to Claro Que Si! To subscribe, text the letter "J". To stop receiving text messages, text "S". For help, text "H". Msg & data rates may apply.');
  $prefix_msg = 'Claro system. ';
  $msg = trim($msg);
  if ($msg == '') {
    $ret = t("Empty request.") . ' ' . $help_msg;
    //log the error
    claro_log("Empty message received. Call ID: ".$call_id);
  }
  else {
    $request = explode(" ", $msg);
    $cmd = strtoupper($request[0]);
    if($cmd == 'Y') {
      $nids = VoipNumber::getNidsByNumber($caller_number);
      $phone_number = claro_get_phone_number($caller_number);
      /*$voip_numbers = voipnumber_get_voip_numbers($caller_number, 'number');
      if(is_array($voip_numbers) && isset($voip_numbers[0])) {
        $vn = $voip_numbers[0];
        $vn->setVerified();
        $vn->save();
        $ret = ''; //success, no need for reply?
      }*/
      //@todo: if number is not published then resend sms
      watchdog('debug', print_r($phone_number,true));
      if($phone_number) {
        $phone_number->status = 1; //verify
        node_save($phone_number);
        $ret = variable_get('claro_sms_welcome', 'Welcome to Claro Que Si').' '.$help_msg;
      }
      else {
        $ret = t('Phone number not recognised.');
      }
    }
    else if($cmd == 'S') {
      //Stop getting messages
      $nids = VoipNumber::getNidsByNumber($caller_number);
      if ($nid = reset($nids)) {
        node_delete($nid);
        $ret = t('Number removed from the list.');
      }
      else {
        $ret = t('Number not in the list.');
      }
    }
    else if($cmd == 'J') {
      if (claro_get_phone_number($caller_number)) {
        //Number already exists
        $ret = t('Number already registered.');
      }
      else {
        $node->type = 'phone_number';
        $node->title = $caller_number;
        $node->language = 'und';
        $node->status = 1;
        $node->field_phone_number['und'][0]['vnid'] = $caller_number;
        $language = claro_number_to_lang($dest_number);
        $node->field_language['und'][0]['value'] = $language;
        // create the node
        node_save($node);
        //@todo: verify number
        if($node->nid){
          $ret = t('You\'re connected to Claro Que Sí! To stop receiving updates, text S. For help, text H');
        }
        else {
          watchdog('claro', 'Error saving phone number: '.print_r($node, true), array(), WATCHDOG_ERROR);
          $ret = t('System error. Please try again later.');
        }
      }
    }
    else if($cmd == 'H') {
      $ret = $help_msg;
    }
    else {
      $ret = t('Unknown command. ') . $help_msg;
    }
  }

  return $prefix_msg.$ret;
}

function claro_log($message) {
  watchdog('claro', $message);
}

function claro_get_episode($nid) {
  watchdog('debug', $nid);
  $n = node_load($nid);
  if(!$n) {
    return NULL;
  }
  $episode = array();
  $episode['nid'] = $n->nid;
  $episode['changed'] = $n->field_episode_date['und'][0]['value'];

  $field_audio = $n->field_audio;
  if ($field_audio['und'][0]) {
    $audio_url = file_create_url($field_audio['und'][0]['uri']);
    $episode['body'] = $audio_url;
  }

  if (!$episode['body']) {
    //If no audio then load text version
    $episode['body'] = $n->body['und'][0]['value'];
  }
  return $episode;
}
/*
* Returns array of claro nodes
*/
function _claro_get_episodes() {
  $episodes = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'episode')
    ->propertyCondition('status', 1)
    ->fieldOrderBy('field_episode_date', 'value', 'DESC')
    ->addMetaData('account', user_load(1)); // Run the query as user 1.
  $result = $query->execute();
  if (isset($result['node'])) {
    $episode_nids = array_keys($result['node']);
    $episode_items = node_load_multiple($episode_nids);
    foreach ($episode_items as $n) {
      $episode = array();
      $episode['nid'] = $n->nid;
      $episode['changed'] = $n->field_episode_date['und'][0]['value'];

      $field_audio = $n->field_audio;
      if ($field_audio['und'][0]) {
        $audio_url = file_create_url($field_audio['und'][0]['uri']);
        $episode['body'] = $audio_url;
      }

      if (!$episode['body']) {
        //If no audio then load text version
        $episode['body'] = $n->body['und'][0]['value'];
      }

      $episodes[] = $episode;
    }
  }

  /*$result = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', 'episode')
    ->condition('status', 0, '<>')
    ->orderBy('changed', 'DESC')
    ->execute();

  foreach ($result as $o) {
    $n = node_load($o->nid);
    $episode = array();
    $episode['nid'] = $n->nid;
    $episode['changed'] = $n->field_episode_date['und'][0]['value'];

    $field_audio = $n->field_audio;
    if ($field_audio['und'][0]) {
      $audio_url = file_create_url($field_audio['und'][0]['uri']);
      $episode['body'] = $audio_url;
    }

    if (!$episode['body']) {
      //If no audio then load text version
      $episode['body'] = $n->body['und'][0]['value'];
    }

    $episodes[] = $episode;
  }
*/
  return $episodes;
}
