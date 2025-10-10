using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class narudzbe : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikIdId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIdId",
                table: "Narudzba");

            migrationBuilder.RenameColumn(
                name: "KurirskaSluzbaIdId",
                table: "Narudzba",
                newName: "KurirskaSluzbaId");

            migrationBuilder.RenameColumn(
                name: "KorisnikIdId",
                table: "Narudzba",
                newName: "KorisnikId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KurirskaSluzbaIdId",
                table: "Narudzba",
                newName: "IX_Narudzba_KurirskaSluzbaId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KorisnikIdId",
                table: "Narudzba",
                newName: "IX_Narudzba_KorisnikId");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba",
                column: "KorisnikId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba",
                column: "KurirskaSluzbaId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikId",
                table: "Narudzba");

            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaId",
                table: "Narudzba");

            migrationBuilder.RenameColumn(
                name: "KurirskaSluzbaId",
                table: "Narudzba",
                newName: "KurirskaSluzbaIdId");

            migrationBuilder.RenameColumn(
                name: "KorisnikId",
                table: "Narudzba",
                newName: "KorisnikIdId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KurirskaSluzbaId",
                table: "Narudzba",
                newName: "IX_Narudzba_KurirskaSluzbaIdId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KorisnikId",
                table: "Narudzba",
                newName: "IX_Narudzba_KorisnikIdId");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KorisnikIdId",
                table: "Narudzba",
                column: "KorisnikIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIdId",
                table: "Narudzba",
                column: "KurirskaSluzbaIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }
    }
}
