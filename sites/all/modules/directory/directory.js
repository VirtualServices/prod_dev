if (Drupal.jsEnabled) {
  $(document).ready(function () {
    $("div.directory-home-vocabulary-collapsed > div.item-list > ul").hide();
    $("div.directory-home-vocabulary-collapsible h3").after('<span class="directory-home-toggle-link">' + toggleT + '</span>');
    $(".directory-home-toggle-link").click(function(){
      $(this).next("ul").toggle();
    });
  });
}

