<?php
include './mysql/mysqlConnect.php';
$sql = "SELECT * FROM users";
$result = $GLOBALS["db.connection"]->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
    ?>
    <div class="panel panel-info">
        <div class="panel-heading">Users</div>
        <div class="panel-body">
            <table class="table">
                <thead>
                    <tr>
                        <th scope="col">ID</th>
                        <th scope="col">Username</th>
                        <th scope="col">Nome</th>
                        <th scope="col">password</th>
                        <th></th>
                    </tr>
                </thead>
                <tbody>
              <?php
              while($row = $result->fetch_assoc())           
              {
                ?>
                    <tr>
                        <th scope="row"><?php echo $row["id"] ?></td>
                        <td><?php echo $row["username"] ?></td>
                        <td><?php echo $row["password_tmp"] ?></td>
                        <td>
                            <a href="deleteUser.php?id=<?php echo $row["id"] ?>"><span class="glyphicon glyphicon-remove"></span></a>
                        </td>

                    </tr>

                <?php
              }

              ?>
                </tbody>
            </table>
        </div>
    </div>
  <?php
} else {
    echo "0 resultados";
}

    include './mysql/mysqlClose.php';
?>